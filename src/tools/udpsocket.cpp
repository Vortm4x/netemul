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
#include "udpsocket.h"
#include "smartdevice.h"
#include "routemodel.h"
#include "udppacket.h"

UdpSocket::UdpSocket(SmartDevice *d, quint16 port) : AbstractSocket(d)
{
    myBindPort = port;
}

void UdpSocket::write(IpAddress address, quint16 port, QByteArray data)
{
    if ( address.isFull() ) {
        writeBroadcast(port,data);
        return;
    }
    IpAddress gw;
    RouteRecord *r = dev->routeModel()->recordAt(address);
    if ( !r ) return;
    if ( r->gateway != r->out ) gw = r->gateway;
    if ( !dev->ipToAdapter(r->out) || !dev->ipToAdapter(r->out)->isConnect() ) return;
    while ( quint32 size = data.size() ) {
        QByteArray tempArray;
        if ( size >= PACKET_SIZE ) tempArray = data.left(PACKET_SIZE);
        else tempArray = data;
        data.remove(0,tempArray.size());
        IpPacket p( r->out, address );
        p.setUpProtocol(IpPacket::udp);
        UdpPacket udp;
        udp.setReceiver(port);
        udp.setSender(myBindPort);
        udp.pack(tempArray);
        p.pack(udp.toData());
        dev->ipToAdapter(r->out)->sendPacket(p,gw);
    }
    if ( m_isAutoDelete ) {
        emit imFinished(this);
    }
}

void UdpSocket::treatPacket(IpPacket p)
{
    UdpPacket udp(p.unpack());
    emit readyRead( udp.unpack() );
}

void UdpSocket::writeBroadcast(quint16 port, QByteArray data)
{
    Q_ASSERT( data.size() <= PACKET_SIZE ); // Нельзя рассылать широковешательно много данных
    foreach ( Interface *i , dev->interfaces() ) {
        if ( !i->isConnect() ) continue;
        IpPacket p( i->ip() , IpAddress::full() );
        p.setUpProtocol(IpPacket::udp);
        UdpPacket udp;
        udp.setReceiver(port);
        udp.setSender(myBindPort);
        udp.pack(data);
        p.pack(udp.toData());
        i->sendPacket(p);
    }
}

