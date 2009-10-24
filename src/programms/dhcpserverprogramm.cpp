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
#include "dhcpserverprogramm.h"
#include "dhcpserverproperty.h"
#include "dhcppacket.h"
#include "smartdevice.h"
#include "udpsocket.h"
#include "udppacket.h"
#include "dhcpservermodel.h"
#include <QMessageBox>

dhcpServerProgramm::dhcpServerProgramm()
{
    myName = tr("DHCP server");
    myDhcpModel = new dhcpServerModel;
}

dhcpServerProgramm::~dhcpServerProgramm()
{
    delete myDhcpModel;
}

void dhcpServerProgramm::setDevice(smartDevice *s)
{
    programmRep::setDevice(s);
    receiver = new udpSocket(device, SERVER_SOCKET);
    receiver->setBind("0.0.0.0");
    foreach ( interface *i, device->interfaces() ) {
        if ( i->isConnect() ) myInterface = i->name();
        break;
    }
    connect( receiver , SIGNAL(readyRead(QByteArray)), SLOT(execute(QByteArray)));
}

void dhcpServerProgramm::setInterface(QString inter)
{
    myInterface = inter;
}

void dhcpServerProgramm::execute(QByteArray data)
{
    qDebug() << "YA TUT!!!!";
    dhcpPacket packet(data);
    dhcpPacket dhcp;
    bool canSend = false;
    if ( packet.type() == dhcpPacket::DHCPDISCOVER ) {
        staticDhcpRecord *rec = myDhcpModel->recordWithMac(packet.chaddr());
        if ( rec ) dhcp = buildOffer( rec,packet.xid() );
        else if ( myDynamic ) {
            clientState *client = chooseDynamic(packet.chaddr(),packet.xid());
            if ( !client ) return;
            dhcp = createDhcpPacket(client,dhcpPacket::DHCPOFFER);
        }
        canSend = true;
    }
    if (packet.type() == dhcpPacket::DHCPREQUEST ) {
        clientState *client = findClient( packet.xid() );
        if ( !client )  return;
        dhcp = createDhcpPacket( client, dhcpPacket::DHCPACK );
        canSend = true;
    }    
    if ( !canSend ) return;
    udpPacket udp;
    udp.setSender( SERVER_SOCKET );
    udp.setReceiver( CLIENT_SOCKET );
    udp.pack( dhcp.toData() );
    ipPacket p( device->adapter(myInterface)->ip(), ipAddress::full() );
    p.pack( udp.toData() );
    p.setUpProtocol( ipPacket::udp );
    device->adapter(myInterface)->sendPacket(p);
}

void dhcpServerProgramm::showProperty()
{
    dhcpServerProperty *d = new dhcpServerProperty(device);
    d->setProgramm(this);
    d->exec();
}

/*!
  Выбираем адрес из динамического диапазона.
  @return указатель на созданную запись.
  */
clientState* dhcpServerProgramm::chooseDynamic(macAddress mac,int id)
{    
    if ( myBeginIp > myEndIp ) {
        qDebug() << myBeginIp.toString() << myEndIp.toString();
        QMessageBox::warning(0,tr("Wrong range"),tr("You enter a wrong range of ip."), QMessageBox::Ok, QMessageBox::Ok);
        return NULL;
    }
    clientState *cl = new clientState;
    bool isContains = false;
    quint32 i = myBeginIp.toInt();
    while ( i <= myEndIp.toInt() ) {
        isContains = myDhcpModel->containRecord( ipAddress(i) );
        foreach ( clientState *j, clients )
            if ( j->ip == i ) {
                isContains = true;
                break;
            }
        if ( isContains ) {
            i++;
            isContains = false;
        }
        else {
            cl->ip = ipAddress(i);
            break;
        }
    }
    if ( cl->ip.isEmpty() ) return NULL;
    cl->mac = mac;
    cl->xid = id;
    cl->time = myTime;
    cl->mask = myMask;
    cl->gateway = myGateway;
    clients << cl;
    return cl;
}

dhcpPacket dhcpServerProgramm::buildOffer(staticDhcpRecord *rec, int id)
{
    clientState *c = new clientState(rec);
    c->xid = id;
    clients << c;
    return createDhcpPacket( c, dhcpPacket::DHCPOFFER );
}

/*!
  */
dhcpPacket dhcpServerProgramm::createDhcpPacket( clientState *client, int state ) const
{
    dhcpPacket p;
    p.setType( state );
    p.setXid( client->xid );
    p.setChaddr( client->mac );
    p.setYiaddr( client->ip );
    p.setMask( client->mask );
    p.setGateway( client->gateway );
    p.setSiaddr( device->adapter(myInterface)->ip() );
    p.setTime( client->time );
    return p;
}
//------------------------------------------------------------

/*! Ищет в списке клиента с данным идентификатрором
  @param xid - идентификатрор.
  @return указатель на запись из списка, если xid совпали, или NULL в противном случае.
  */
clientState* dhcpServerProgramm::findClient(int xid) const
{
    foreach ( clientState *i, clients )
        if ( i->xid == xid ) return i;
    return NULL;
}
//------------------------------------------------------------

/*!
  Записывает отличительные черты в поток.
  @param stream - поток для записи.
*/
void dhcpServerProgramm::write(QDataStream &stream) const
{
    stream << DHCPServer;
    programmRep::write(stream);
    myDhcpModel->write(stream);
    stream << myInterface;
    stream << myBeginIp;
    stream << myEndIp;
    stream << myMask;
    stream << myGateway;
    stream << myTime;
    stream << myDynamic;
}
//---------------------------------------------------
/*!
  Считывает отличительные черты из потока.
  @param stream - поток для чтения.
*/
void dhcpServerProgramm::read(QDataStream &stream)
{
    programmRep::read(stream);
    myDhcpModel->read(stream);
    stream >> myInterface;
    stream >> myBeginIp;
    stream >> myEndIp;
    stream >> myMask;
    stream >> myGateway;
    stream >> myTime;
    stream >> myDynamic;
}
//---------------------------------------------------

//---------------------------------------------------
clientState::clientState(staticDhcpRecord *rec)
{
    ip = rec->yiaddr;
    mac = rec->chaddr;
    mask = rec->mask;
    gateway = rec->gateway;
    time = rec->time;
}

