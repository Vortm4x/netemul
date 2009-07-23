#include "tcppacket.h"

tcpPacket::tcpPacket()
{
}

tcpPacket::tcpPacket(const QByteArray &b) : udpPacket(b)
{

}

/*!
  Записывает tcp-сегмент в поток.
  @param stream - поток для записи.
  @param p - записываемый пакет.
  @return ссылку на результирующий поток.
*/
QDataStream& operator<<( QDataStream &stream, const tcpPacket &p )
{
    stream << p.myReceiver;
    stream << p.mySender;
    stream << p.data;
    return stream;
}
//-------------------------------------------------------
/*!
  Извлекает из потока tcp-сегмент
  @param stream - поток для чтения.
  @param p - извлекаемый пакет.
  @return ссылку на результирующий поток.
*/
QDataStream& operator>>( QDataStream &stream, tcpPacket &p )
{
    stream >> p.myReceiver;
    stream >> p.mySender;
    stream >> p.data;
    return stream;
}
