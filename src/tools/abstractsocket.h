#ifndef ABSTRACTSOCKET_H
#define ABSTRACTSOCKET_H

#include "ippacket.h"

class smartDevice;

class abstractSocket
{
public:
    abstractSocket(smartDevice *d);
    ~abstractSocket();
    bool isOurData(ipAddress address, quint16 port);
    void setBind(ipAddress address) { myBind = address; }
    ipAddress destination() { return dest; }
    virtual void treatPacket(ipPacket) { }
    virtual void secondEvent() { }
    virtual void write(QByteArray) { }
    virtual void read(QByteArray,int) { }
protected:
    quint16 myBindPort;
    ipAddress myBind;
    ipAddress dest;
    smartDevice *dev;
};

#endif // ABSTRACTSOCKET_H
