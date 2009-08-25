#ifndef ABSTRACTSOCKET_H
#define ABSTRACTSOCKET_H

#include "ippacket.h"

class smartDevice;

class abstractSocket
{
public:
    abstractSocket(smartDevice *d);
    ipAddress destination() { return dest; }
    virtual void treatPacket(ipPacket) { }
    virtual void secondEvent() { }
    virtual void write(QByteArray) { }
protected:
    ipAddress dest;
    smartDevice *dev;
};

#endif // ABSTRACTSOCKET_H
