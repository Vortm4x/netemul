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
#include "udppacket.h"

QByteArray UdpPacket::toData() const
{
    return d->toData();
}

UdpPacket::UdpPacket(const QByteArray &b)
{
    d = new UdpPacketData;
    QDataStream s(b);
    s >> d->receiver >> d->sender >> d->data;
}

QString UdpPacket::typeToString() const
{
    switch ( d->receiver ) {
        case User: return QObject::tr("UDP Message user");
        case RIP: return QObject::tr("RIP");
        case DHCPClient: case DHCPServer: return QObject::tr("DHCP message");
        default: return QObject::tr("None");
    }
}

QString UdpPacket::toString() const
{
    QString temp;
    temp.append("UDP, "+QObject::tr("sender port: %1, receiver port: %2").arg(d->sender).arg(d->receiver) );
    return temp;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

UdpPacketData::UdpPacketData(const UdpPacketData &u) : QSharedData(u)
{
    sender = u.sender;
    receiver = u.receiver;
    data = u.data;
}

QByteArray UdpPacketData::toData() const
{
    QByteArray t;
    QDataStream s(&t,QIODevice::WriteOnly);
    s << receiver << sender << data;
    return t;
}
