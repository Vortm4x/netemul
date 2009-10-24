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

ipPacket::ipPacket(const QByteArray &b)
{
    d = new ipPacketData;
    QDataStream s(b);
    s >> d->sender >> d->receiver >> d->upProtocol >> d->data >> d->ttl;
}

ipPacket::ipPacket(ipAddress s,ipAddress r)
{
    d = new ipPacketData;
    d->sender = s ;
    d->receiver = r;
}
/*!
  Проверяет является ли пакет широковещательным.
  @param mask - маска сети, по которой идет проверка.
  @return - true - широковещательный, false - в противном случае.
*/
bool ipPacket::isBroadcast(const ipAddress mask) const
{
    ipAddress a = ~mask;
    if ( ( d->receiver & a ) == a) return true;
    return false;
}
//----------------------------------------------------
QByteArray ipPacket::toData() const
{
    return d->toData();
}

QString ipPacket::toString() const
{
    QString temp;
    temp.append(QObject::tr("IP packet, sender: %1, receiver: %2 TTL: %3").arg(d->sender.toString()).arg(d->receiver.toString()).arg(d->ttl));
    return temp;
}

quint16 ipPacket::receiverSocket() const
{
    QDataStream stream(d->data);
    quint16 t;
    stream >> t;
    return t;
}

quint8 ipPacket::decTtl()
{
    d->ttl--;
    return d->ttl;
}
//----------------------------------------------------
//----------------------------------------------------

/*!
  * Переводит пакет в массив байт.
*/
QByteArray ipPacketData::toData() const
{
    QByteArray t;
    QDataStream s(&t,QIODevice::WriteOnly);
    s << sender << receiver << upProtocol << data << ttl;
    return t;
}

/*!
 * Копирующий конструктор данных.
*/
ipPacketData::ipPacketData(const ipPacketData &other) : QSharedData(other)
{
    sender = other.sender;
    receiver = other.receiver;
    data = other.data;
    upProtocol = other.upProtocol;
    ttl = other.ttl;
}

