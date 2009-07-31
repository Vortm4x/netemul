#ifndef TCPPACKET_H
#define TCPPACKET_H

#include <QDataStream>
#include "udppacket.h"

/*!
  Реализует tcp-сегмент
*/
class tcpPacket : public udpPacket
{
public:
    tcpPacket();
    tcpPacket(const QByteArray &b);
protected:
    friend QDataStream& operator<<( QDataStream &stream, const tcpPacket &p );
};

#endif // TCPPACKET_H
