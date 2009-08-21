#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include "ipaddress.h"
#include "ippacket.h"
#include "tcppacket.h"

class smartDevice;

class tcpSocket
{
public:
    enum { Sequence = 5000 };
    tcpSocket(smartDevice *d,ipAddress a);
    void setSize(int s) { size = s; }    
    void setConnection() const;
    void treatPacket(ipPacket p);
    void confirmConnection(ipPacket p);
    ipAddress destination() { return dest; }
private:    
    void sendMessage(ipPacket p) const;
    tcpPacket createPacket(quint16 sender, quint16 receiver, quint32 sequence, quint32 ack, quint8 flag) const;
    smartDevice *dev;
    int size;
    ipAddress dest;
};

#endif // TCPSOCKET_H
