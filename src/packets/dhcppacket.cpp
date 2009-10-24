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
#include "dhcppacket.h"

dhcpPacket::dhcpPacket()
{
    d = new dhcpPacketData;
}

dhcpPacket::dhcpPacket(const QByteArray &data)
{
    d = new dhcpPacketData;
    QDataStream s(data);
    s >> d->xid >> d->type >> d->yiaddr >> d->siaddr >> d->chaddr >> d->mask >> d->gateway >> d->time;
}
/*!
  * Выполняет сериализацию.
  */
QByteArray dhcpPacket::toData() const
{
    return d->toData();
}
//-----------------------------------------------------------------------------------------
/*!
  * Преобразует тип пакета в строковое представление.
  */
QString dhcpPacket::typeString() const
{
    const QString names[] = { "DHCPDISCOVER" , "DHCPOFFER" , "DHCPREQUEST" , "DHCPACK" };
    return names[ d->type ];
}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
dhcpPacketData::dhcpPacketData(const dhcpPacketData &other) : QSharedData(other)
{
    xid = other.xid;
    type = other.type;
    yiaddr = other.yiaddr;
    siaddr = other.siaddr;
    chaddr = other.chaddr;
    mask = other.mask;
    gateway = other.gateway;
    time = other.time;
}
/*!
  * Преобразует содержимое в последовательность байт.
  */
QByteArray dhcpPacketData::toData() const
{
    QByteArray d;
    QDataStream s(&d, QIODevice::WriteOnly);
    s << xid << type << yiaddr << siaddr << chaddr << mask << gateway << time;
    return d;
}
//-------------------------------------------------------------------------------------------
