#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include "abstractsocket.h"

class udpSocket : public abstractSocket
{
public:
    enum { PACKET_SIZE = 1024 };
    udpSocket(smartDevice *d,ipAddress address, quint16 port);
    void write(QByteArray data);
private:
    quint32 myPort;
};

#endif // UDPSOCKET_H
