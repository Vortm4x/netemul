#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include "abstractsocket.h"
#include "tcppacket.h"

class tcpSocket : public abstractSocket
{
    Q_OBJECT
public:
    enum { Sequence = 5000, PACKET_SIZE = 1024 };
    tcpSocket() { }
    tcpSocket(smartDevice *d,quint16 port);
    ~tcpSocket();
    void setConnection();
    void treatPacket(ipPacket p);
    void confirmConnection(ipPacket p);
    void secondEvent();
    void write(ipAddress a, quint16 p, QByteArray data);
signals:
    void writeEnd();
    void receiveEnd();
private:
    void sendMessage(ipPacket p) const;
    void sendWindow();
    tcpPacket createPacket( quint32 sequence, quint32 ack, quint8 flag) const;  
    quint16 myReceiverPort;
    quint32 seq;
    int timeout;
    int time;
    int inputTime;
    int panicTime;
    int lastNum;
    bool isEnd;
    bool isConnected;
    QList<tcpPacket> buffer;
};

#endif // TCPSOCKET_H
