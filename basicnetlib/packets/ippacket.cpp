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
#include "ippacket.h"

IpPacket::IpPacket(const QByteArray &b)
{
    d = new IpPacketData;
    QDataStream s(b);
    s >> d->sender >> d->receiver >> d->upProtocol >> d->data >> d->ttl;
}

IpPacket::IpPacket(IpAddress s,IpAddress r)
{
    d = new IpPacketData;
    d->sender = s ;
    d->receiver = r;
    d->ttl = 64;
}
/*!
  Проверяет является ли пакет широковещательным.
  @param mask - маска сети, по которой идет проверка.
  @return - true - широковещательный, false - в противном случае.
*/
bool IpPacket::isBroadcast(const IpAddress mask) const
{
    IpAddress a = ~mask;
    if ( ( d->receiver & a ) == a) return true;
    return false;
}
//----------------------------------------------------
QByteArray IpPacket::toData() const
{
    return d->toData();
}

QString IpPacket::toString() const
{
    QString temp;
    temp.append(QObject::tr("IP packet, sender: %1, receiver: %2 TTL: %3").arg(d->sender.toString()).arg(d->receiver.toString()).arg(d->ttl));
    return temp;
}

quint16 IpPacket::receiverSocket() const
{
    QDataStream stream(d->data);
    quint16 t;
    stream >> t;
    return t;
}

quint8 IpPacket::decTtl()
{
    d->ttl--;
    return d->ttl;
}
//----------------------------------------------------
//----------------------------------------------------

/*!
  * Переводит пакет в массив байт.
*/
QByteArray IpPacketData::toData() const
{
    QByteArray t;
    QDataStream s(&t,QIODevice::WriteOnly);
    s << sender << receiver << upProtocol << data << ttl;
    return t;
}

/*!
 * Копирующий конструктор данных.
*/
IpPacketData::IpPacketData(const IpPacketData &other) : QSharedData(other)
{
    sender = other.sender;
    receiver = other.receiver;
    data = other.data;
    upProtocol = other.upProtocol;
    ttl = other.ttl;
}

