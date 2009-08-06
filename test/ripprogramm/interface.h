#ifndef INTERFACE_H
#define INTERFACE_H

#include "ippacket.h"
#include "udppacket.h"

class interface
{
public:
    interface();
    void setIp(ipAddress a) { myIp = a; }
    void setMask(ipAddress a) { myMask = a; }
    QByteArray buffer() { return buf; }
    bool isConnect() { return true; }
    ipAddress mask() const { return myMask; }
    ipAddress ip() const { return myIp; }
    void sendPacket(ipPacket p) { buf = udpPacket(p.unpack()).unpack();  }
private:
    ipAddress myIp;
    ipAddress myMask;
    QByteArray buf;
};

#endif // INTERFACE_H
