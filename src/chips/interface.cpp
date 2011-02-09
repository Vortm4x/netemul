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

Interface::Interface(QObject *parent) : AbstractChip(parent)
{
    mySocket = new DevicePort(this);
    connect( mySocket , SIGNAL(cableConnected(Cable*)) , SIGNAL(cableConnected(Cable*)) );    
}

Interface* Interface::create(QObject *parent)
{
    Interface *u = new Interface(parent);
    u->setArpModel(new ArpModel(u) );
    return u;
}

Interface::~Interface()
{    
    delete myArpTable;
    qDeleteAll(myWaits);
}

void Interface::pushToSocket(Frame &f)
{
    checkSend(f);
    emit sendData(f,myName);
    mySocket->pushToSend(f);
}

void Interface::receiveEvent(Frame &fr,DevicePort*)
{
    checkReceive(fr);
    emit receiveData(fr,myName);
    if ( fr.type() == Frame::ip ) {
        IpPacket p(fr.unpack());
        emit receivedPacket(p);
    }
    if ( fr.type() == Frame::arp ) {
        ArpPacket p(fr.unpack());
        receiveArp(p);
    }
}

void Interface::sendBroadcast(IpPacket &p)
{
    Frame f = createFrame( MacAddress("FF:FF:FF:FF:FF:FF") , Frame::ip );
    f.pack( p.toData() );
    pushToSocket(f);
}

void Interface::sendPacket(IpPacket &p,IpAddress gw /* = ipAddress("0.0.0.0") */ )
{
    if ( p.isBroadcast( myMask ) ) {
        sendBroadcast(p);
        return;
    }
    IpAddress t;
    if ( gw.isEmpty() ) t = p.receiver();
    else t = gw;
    ArpRecord *a = myArpTable->recordAt(t);
    if ( a ) {
        Frame f = createFrame( a->mac , Frame::ip );
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

Frame Interface::createFrame( MacAddress receiverMac, int t)
{
    Frame f;
    f.setSender(myMac);
    f.setReceiver(receiverMac);
    f.setType(t);
    return f;
}

void Interface::receiveArp(ArpPacket &arp)
{
    if ( arp.type() == ArpPacket::response ) {
        if ( arp.senderIp() == myIp ) {
            emit equalIpDetected();
            QMessageBox::warning(0, QObject::tr("The network is not working correctly"),
                                 QObject::tr("The network found a matching IP address"),
                                 QMessageBox::Ok, QMessageBox::Ok);
            return;
        }
        myArpTable->addToTable(  arp.senderIp() , arp.senderMac() , ArpModel::dinamicMode );
        foreach ( waitPacket *i , myWaits )
            if ( i->dest == arp.senderIp() ) {
                foreach ( IpPacket p, i->packets ) sendPacket(p,arp.senderIp());
                myWaits.removeOne(i);
                delete i;
                break;
            }
    }
    else {
        ArpRecord *t = myArpTable->addToTable(arp.senderIp() , arp.senderMac() , ArpModel::dinamicMode );
        if ( arp.receiverIp() == myIp ) sendArpResponse(t->mac, t->ip);
    }
}

bool Interface::isConnect() const
{
    return mySocket->isConnect();
}

Cable* Interface::socketCable() const
{
    return mySocket->cable();
}

bool Interface::isCableConnect(const Cable *c) const
{
    return mySocket->isCableConnect(c);
}

void Interface::setChecked(bool b)
{
    mySocket->setChecked(b);
}

void Interface::deciSecondEvent()
{
    mySocket->queueEvent();
    if ( mySocket->hasReceive() ) {
        Frame f = mySocket->popFromReceive();
        receiveEvent( f, mySocket );
    }
}

void Interface::secondEvent()
{
    myArpTable->update();
    foreach ( waitPacket *i , myWaits ) {
        if ( --i->time ) continue;
        if ( i->count <= COUNT_AGAINST_SEND ) {
            i->count++;
            i->time = qrand()%(AppSetting::arpResponceTime()*i->count)+AppSetting::arpResponceTime();
            sendArpRequest( i->dest );
        } else {
            myWaits.removeOne(i);
            delete i;
        }
    }
}

void Interface::sendArpRequest(IpAddress a)
{
    if ( a.isEmpty() ) return;
    ArpPacket p(  MacAddress() , myMac , a , myIp , ArpPacket::request );
    MacAddress m;
    m.setBroadcast();
    Frame f = createFrame(m, Frame::arp);
    f.pack(p.toData());
    pushToSocket(f);
}

void Interface::sendArpResponse(MacAddress m, IpAddress a)
{
    ArpPacket p(m, myMac, a, myIp, ArpPacket::response);
    Frame f = createFrame(m, Frame::arp);
    f.pack(p.toData());
    pushToSocket(f);
}

void Interface::write(QDataStream &stream) const
{
    AbstractChip::write(stream);
    stream << myName;
}

void Interface::read(QDataStream &stream)
{
    AbstractChip::read(stream);
    stream >> myName;
}

bool Interface::isBusy() const
{
    return mySocket->isBusy();
}

int Interface::trafficDigit() const
{
    int sum = 0;
    foreach ( waitPacket *i , myWaits ) sum += i->packets.size();
    return mySocket->trafficDigit() + sum;
}
//-------------------------------------------------------
//-------------------------------------------------------

waitPacket* waitPacket::create(IpAddress a,IpPacket p)
{
    waitPacket *t = new waitPacket;
    t->dest = a;
    t->packets << p;
    t->time = AppSetting::arpResponceTime();
    t->count = 0;
    return t;
}

