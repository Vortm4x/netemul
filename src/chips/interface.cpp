/****************************************************************************************
** NetEmul - program for simulating computer networks.
** Copyright © 2009 Semenov Pavel and Omilaeva Anastasia
**
** NetEmul is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** NetEmul is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with the NetEmul; if not, write to the Free
** Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
** 02111-1307 USA.
****************************************************************************************/
#include <QtCore/QList>
#include <QtGui/QMessageBox>
#include "interface.h"
#include "deviceport.h"
#include "appsetting.h"
#include "arpmodel.h"

interface::interface(const QString &name) : myName(name)
{
    mySocket = new devicePort;
    myArpTable = new arpModel;
}

interface::~interface()
{
    delete mySocket;
    delete myArpTable;
    qDeleteAll(myWaits);
}

void interface::pushToSocket(frame &f)
{
    checkSend(f);
    emit sendData(f,myName);
    mySocket->pushToSend(f);
}

void interface::receiveEvent(frame &fr,devicePort*)
{
    checkReceive(fr);
    emit receiveData(fr,myName);
    if ( fr.type() == frame::ip ) {
        ipPacket p(fr.unpack());
        emit receivedPacket(p);
    }
    if ( fr.type() == frame::arp ) {
        arpPacket p(fr.unpack());
        receiveArp(p);
    }
}

void interface::sendBroadcast(ipPacket &p)
{
    frame f = createFrame( macAddress("FF:FF:FF:FF:FF:FF") , frame::ip );
    f.setDifferent( frame::broadcast );
    f.pack( p.toData() );
    pushToSocket(f);
}

void interface::sendPacket(ipPacket &p,ipAddress gw /* = ipAddress("0.0.0.0") */ )
{
    if ( p.isBroadcast( myMask ) ) {
        sendBroadcast(p);
        return;
    }
    ipAddress t;    
    if ( gw.isEmpty() ) t = p.receiver();
    else t = gw;
    arpRecord *a = myArpTable->recordAt(t);
    if ( a ) {
        frame f = createFrame( a->mac , frame::ip );
        a->time = 0; // Стартуем заново время жизни arp записи
        f.pack( p.toData() );
        pushToSocket(f);
        return;
    }
    foreach ( waitPacket *i , myWaits )
        if ( i->dest == t ) {
            i->insert(p);
            return;
        }
    sendArpRequest(t);
    myWaits << waitPacket::create(t,p);
}

frame interface::createFrame( macAddress receiverMac, int t)
{
    frame f;
    f.setSender(myMac);
    f.setReceiver(receiverMac);
    f.setType(t);
    return f;
}

void interface::receiveArp(arpPacket &arp)
{
    if ( arp.type() == arpPacket::response ) {
        if ( arp.receiverIp() == arp.senderIp() ) {
            QMessageBox::warning(0, QObject::tr("The network is not working correctly"),
                                 QObject::tr("The network found a matching IP address"),
                                 QMessageBox::Ok, QMessageBox::Ok);
        }
        myArpTable->addToTable(  arp.senderIp() , arp.senderMac() , arpModel::dinamicMode );
        foreach ( waitPacket *i , myWaits )
            if ( i->dest == arp.senderIp() ) {
                foreach ( ipPacket p, i->packets ) sendPacket(p,arp.senderIp());
                myWaits.removeOne(i);
                delete i;
                break;
            }
    }
    else {
        arpRecord *t = myArpTable->addToTable(arp.senderIp() , arp.senderMac() , arpModel::dinamicMode );
        if ( arp.receiverIp() == myIp ) sendArpResponse(t->mac, t->ip);
    }
}

bool interface::isConnect() const
{
    return mySocket->isConnect();
}

void interface::setConnect(bool b,cableDev *c)
{
    mySocket->setConnect(b,c);
}

bool interface::isCableConnect(const cableDev *c) const
{
    return mySocket->isCableConnect(c);
}

void interface::setChecked(bool b)
{
    mySocket->setChecked(b);
}

void interface::deciSecondEvent()
{
    mySocket->queueEvent();
    if ( mySocket->hasReceive() ) {
        frame f = mySocket->popFromReceive();
        receiveEvent( f, mySocket );
    }
}

void interface::secondEvent()
{
    myArpTable->update();
    int time = appSetting::arpResponceTime()+qrand()%20;
    foreach ( waitPacket *i , myWaits ) {
        if ( ++i->time < time ) continue;
        if ( i->count <= COUNT_AGAINST_SEND ) {
            i->time = 0;
            i->count++;
            sendArpRequest( i->dest );
        } else {
            myWaits.removeOne(i);
            delete i;
        }
    }
}

void interface::sendArpRequest(ipAddress a)
{
    if ( a.isEmpty() ) return;
    arpPacket p(  macAddress() , myMac , a , myIp , arpPacket::request );
    macAddress m;
    m.setBroadcast();
    frame f = createFrame(m, frame::arp);
    f.setDifferent(frame::broadcast);
    f.pack(p.toData());
    pushToSocket(f);
}

void interface::sendArpResponse(macAddress m, ipAddress a)
{
    arpPacket p(m, myMac, a, myIp, arpPacket::response);
    frame f = createFrame(m, frame::arp);
    f.pack(p.toData());
    f.setDifferent(frame::normal);
    pushToSocket(f);
}

void interface::write(QDataStream &stream) const
{
    abstractChip::write(stream);
    stream << myName;
}

void interface::read(QDataStream &stream)
{
    abstractChip::read(stream);
    stream >> myName;
}

bool interface::isBusy() const
{
    return mySocket->isBusy();
}

int interface::trafficDigit() const
{
    int sum = 0;
    foreach ( waitPacket *i , myWaits ) sum += i->packets.size();
    return mySocket->trafficDigit() + sum;
}
//-------------------------------------------------------
//-------------------------------------------------------

waitPacket* waitPacket::create(ipAddress a,ipPacket p)
{
    waitPacket *t = new waitPacket;
    t->dest = a;
    t->packets << p;
    t->time = t->count = 0;
    return t;
}

