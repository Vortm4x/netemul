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
    stream << p.toData();
    return stream;
}
//-------------------------------------------------------
