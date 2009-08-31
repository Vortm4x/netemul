#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include "abstractsocket.h"
#include <QQueue>
#include <QByteArray>

class udpSocket : public abstractSocket
{
    Q_OBJECT
public:
    enum { PACKET_SIZE = 1024 };
    udpSocket() { }
    udpSocket(smartDevice *d, quint16 port);
    ~udpSocket() { }
    void treatPacket(ipPacket p);
    void write(ipAddress address, quint16 port, QByteArray data);
};

#endif // UDPSOCKET_H
