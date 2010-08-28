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
#include "arppacket.h"

arpPacket::arpPacket(macAddress rm , macAddress sm , IpAddress ri , IpAddress si , quint8 t)
{
    d = new arpPacketData;
    d->receiverMac = rm;
    d->senderMac = sm;
    d->receiverIp = ri;
    d->senderIp = si;
    d->type = t;
}

arpPacket::arpPacket(const QByteArray &b)
{
    d = new arpPacketData;
    QDataStream s(b);
    s >> d->type >> d->senderMac >> d->receiverMac >> d->senderIp >> d->receiverIp;
}

QByteArray arpPacket::toData() const
{
    return d->toData();
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

QByteArray arpPacketData::toData() const
{
    QByteArray t;
    QDataStream s(&t,QIODevice::WriteOnly);
    s << type << senderMac << receiverMac << senderIp << receiverIp;
    return t;
}

arpPacketData::arpPacketData(const arpPacketData &other) : QSharedData(other)
{
    receiverMac = other.receiverMac;
    senderMac = other.senderMac;
    receiverIp = other.receiverIp;
    senderIp = other.senderIp;
    type = other.type;
}

