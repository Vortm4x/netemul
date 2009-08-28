#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include "abstractsocket.h"
#include "tcppacket.h"
#include <QMap>

class tcpSocket : public abstractSocket
{
public:
    enum { Sequence = 5000, PACKET_SIZE = 1024 };
    tcpSocket(smartDevice *d,quint16 port);
    void setConnection();
    void treatPacket(ipPacket p);
    void confirmConnection(ipPacket p);
    void secondEvent();
    void write(ipAddress a, quint16 p, QByteArray data);
protected:
    void sendMessage(ipPacket p) const;
    tcpPacket createPacket( quint32 sequence, quint32 ack, quint8 flag) const;
private:    
    quint16 sender;
    quint16 receiver;
    quint32 seq;
    int timeout;
    int time;
    int inputTime;
    int lastNum;
    QMap<quint32,tcpPacket> buffer;
};

#endif // TCPSOCKET_H
