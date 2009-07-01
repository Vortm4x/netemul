#include "ippacket.h"

ipPacket::ipPacket()
{
}

ipPacket ipPacket::operator=(ipPacket other)
{
    mySender = other.mySender;
    myReceiver = other.myReceiver;
    return *this;
}
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
/*!
  Назначает пакету широковещательный адрес исходя из маски.
  @param mask - Маска.
*/
void ipPacket::setBroadcast(const ipAddress mask)
{
    myReceiver = mySender | ~mask;
}
//---------------------------------------------------
/*!
  Записывает ip-пакет в поток.
  @param stream - поток для записи.
  @param p - записываемый пакет.
  @return ссылку на результирующий поток.
*/
QDataStream& operator<<(QDataStream &stream,const ipPacket &p)
{
    stream << p.mySender;
    stream << p.myReceiver;
    stream << p.myUpProtocol;
    return stream;
}
//--------------------------------------------------
/*!
  Извлекает ip-пакет из потока.
  @param stream - поток для чтения.
  @param p - извлекаемый пакет.
  @return ссылку на результирующий поток.
*/
QDataStream& operator>>(QDataStream &stream,ipPacket &p)
{
    stream >> p.mySender;
    stream >> p.myReceiver;
    stream >> p.myUpProtocol;
    return stream;
}
//-------------------------------------------------------------
