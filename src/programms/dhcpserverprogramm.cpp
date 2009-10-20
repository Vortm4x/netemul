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

dhcpServerProgramm::dhcpServerProgramm()
{
    myName = tr("DHCP server");
}

void dhcpServerProgramm::setDevice(smartDevice *s)
{
    programmRep::setDevice(s);
    receiver = new udpSocket(device, SERVER_SOCKET);
    receiver->setBind("0.0.0.0");
    connect( receiver , SIGNAL(readyRead(QByteArray)), SLOT(execute(QByteArray)));
}

void dhcpServerProgramm::setInterface(QString inter)
{
    foreach ( interface *i, device->interfaces() )
        if ( i->name() == inter ) myInterface = i;
}

void dhcpServerProgramm::execute(QByteArray data)
{
    dhcpPacket packet(data);
    xid = packet.xid();
    dhcpPacket dhcp;
    if ( packet.type() == dhcpPacket::DHCPDISCOVER ) {
        if ( myStatics.isEmpty() ) return;
        foreach ( staticRecord *i, myStatics )
            if ( i->chaddr == packet.chaddr() ) dhcp = buildOffer( i );
    }
    udpPacket udp;
    udp.setSender( SERVER_SOCKET );
    udp.setReceiver( CLIENT_SOCKET );
    udp.pack( dhcp.toData() );
    ipPacket p( myInterface->ip(), ipAddress::full() );
    p.pack( udp.toData() );
    p.setUpProtocol( ipPacket::udp );
    myInterface->sendPacket(p);
}

bool dhcpServerProgramm::containRecord(staticRecord *rec)
{
    if ( myStatics.isEmpty() ) return false;
    foreach (staticRecord *i, myStatics)
        if ( i == rec ) return true;
    return false;
}

void dhcpServerProgramm::addStaticRecord(staticRecord *rec)
{
    myStatics << rec;
}

void dhcpServerProgramm::showProperty()
{
    dhcpServerProperty *d = new dhcpServerProperty(device);
    d->setProgramm(this);
    d->exec();
}

dhcpPacket dhcpServerProgramm::buildOffer(staticRecord *rec) const
{
    dhcpPacket p;
    p.setType( dhcpPacket::DHCPOFFER );
    p.setXid( xid );
    p.setChaddr( rec->chaddr );
    p.setYiaddr( rec->yiaddr );
    p.setMask( rec->mask );
    p.setGateway( rec->gateway );
    return p;
}

/*!
  Записывает отличительные черты в поток.
  @param stream - поток для записи.
*/
void dhcpServerProgramm::write(QDataStream &stream) const
{
    stream << DHCPServer;
    programmRep::write(stream);
    stream << myStatics.size();
    foreach ( staticRecord *i , myStatics )
        i->write(stream);
}
//---------------------------------------------------
/*!
  Считывает отличительные черты из потока.
  @param stream - поток для чтения.
*/
void dhcpServerProgramm::read(QDataStream &stream)
{
    programmRep::read(stream);
    int n;
    stream >> n;
    for ( int i = 0 ; i < n ; i++ ) {
        staticRecord *t = new staticRecord;
        t->read(stream);
        myStatics << t;
    }
}
//---------------------------------------------------


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void staticRecord::write(QDataStream &stream) const
{
    stream << chaddr << yiaddr << mask << gateway << time;
}

void staticRecord::read(QDataStream &stream)
{
    stream >> chaddr >> yiaddr >> mask >> gateway >> time;
}
