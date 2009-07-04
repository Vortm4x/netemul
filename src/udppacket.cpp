#include "udppacket.h"

udpPacket::udpPacket()
{
}

/*!
  Записывает udp-дейтаграмму в поток.
  @param stream - поток для записи.
  @param p - записываемый пакет.
  @return ссылку на результирующий поток.
*/
QDataStream& operator<<( QDataStream &stream, const udpPacket &p )
{
    stream << p.myReceiver;
    stream << p.mySender;
    stream << p.myData;
    return stream;
}
//-------------------------------------------------------
/*!
  Извлекает из потока udp-дейтаграмму
  @param stream - поток для чтения.
  @param p - извлекаемый пакет.
  @return ссылку на результирующий поток.
*/
QDataStream& operator>>( QDataStream &stream, udpPacket &p )
{
    stream >> p.myReceiver;
    stream >> p.mySender;
    stream >> p.myData;
    return stream;
}
//-------------------------------------------------------
