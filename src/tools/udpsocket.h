#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include "abstractsocket.h"
#include <QQueue>
#include <QByteArray>

class udpSocket : public abstractSocket
{
public:
    enum { PACKET_SIZE = 1024 };
    udpSocket(smartDevice *d, quint16 port);
    void treatPacket(ipPacket p);
    int sizeReceiveData() const { return buffer.size(); }
    void write(ipAddress address, quint16 port, QByteArray data);
    void read(QByteArray &data);
private:
    QQueue<QByteArray> buffer;
};

#endif // UDPSOCKET_H
