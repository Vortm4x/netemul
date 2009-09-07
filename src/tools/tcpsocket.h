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
    void sendMessage(tcpPacket t) const;
    void sendWindow();
    void sendAck();
    tcpPacket createPacket( quint32 sequence, quint32 ack, quint8 flag) const;  
    quint16 myReceiverPort;
    quint32 sendIsn;
    quint32 receiveIsn;
    quint32 isn;
    int timeout;
    int time;
    int inputTime;
    int panicTime;
    int lastNum;
    QByteArray buffer;
};

#endif // TCPSOCKET_H
