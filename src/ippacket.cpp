#include "ippacket.h"

ipPacket::ipPacket(const QByteArray &b)
{
    d = new ipPacketData;
    QDataStream s(b);
    s >> d->sender >> d->receiver >> d->upProtocol >> d->data;
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

//----------------------------------------------------
//----------------------------------------------------

/*!
  * Переводит пакет в массив байт.
*/
QByteArray ipPacketData::toData() const
{
    QByteArray t;
    QDataStream s(&t,QIODevice::WriteOnly);
    s << sender << receiver << upProtocol << data;
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
}

