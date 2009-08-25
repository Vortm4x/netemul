#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include "abstractsocket.h"

class udpSocket : public abstractSocket
{
public:
    enum { PACKET_SIZE = 1024 };
    udpSocket(smartDevice *d, quint16 port);
    void write(ipAddress address, quint16 port, QByteArray data);
private:
    quint16 myBind;
};

#endif // UDPSOCKET_H
