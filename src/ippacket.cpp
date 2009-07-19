#include "ippacket.h"

ipPacket::ipPacket()
{
}

ipPacket& ipPacket::operator=(const ipPacket &other)
{
    mySender = other.mySender;
    myReceiver = other.myReceiver;
    data = other.data;
    return *this;
}
/*!
  Копирующий конструктор класса.
*/
ipPacket::ipPacket(const ipPacket &other)
{
    mySender = other.mySender;
    myReceiver = other.myReceiver;
    data = other.data;
}
//--------------------------------------------------
/*!
  Проверяет является ли пакет широковещательным.
  @param mask - маска сети, по которой идет проверка.
  @return - true - широковещательный, false - в противном случае.
*/
bool ipPacket::isBroadcast(const ipAddress mask) const
{
    ipAddress a = ~mask;
    if ( ( myReceiver & a ) == a) return true;
    return false;
}
//----------------------------------------------------


