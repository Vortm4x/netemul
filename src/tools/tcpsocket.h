#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include "abstractsocket.h"
#include "tcppacket.h"
#include <QMap>

class tcpSocket : public abstractSocket
{
public:
    enum { Sequence = 5000 };
    tcpSocket(smartDevice *d,ipAddress a,quint16 sp,quint16 rp);
    void setSize(int s);
    void setConnection();
    void treatPacket(ipPacket p);
    void confirmConnection(ipPacket p);
    void secondEvent();
private:    
    void sendMessage(ipPacket p) const;
    tcpPacket createPacket( quint32 sequence, quint32 ack, quint8 flag) const;
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
