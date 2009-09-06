#ifndef DHCPPACKET_H
#define DHCPPACKET_H

#include "ipaddress.h"
#include "macaddress.h"

class dhcpPacket
{
public:
    dhcpPacket();
    void setYiaddr(ipAddress a) { myYiaddr = a; }
    void setSiaddr(ipAddress a) { mySiaddr = a; }
    void setChaddr(macAddress a) { myChaddr = a; }
    void setXid(int x) { myXid = x; }
    int xid() const { return myXid; }
    ipAddress yiaddr() const { return myYiaddr; }
    ipAddress siaddr() const { return mySiaddr; }
    macAddress chaddr() const { return myChaddr; }
private:
    int myXid;
    ipAddress myYiaddr;
    ipAddress mySiaddr;
    macAddress myChaddr;
};

#endif // DHCPPACKET_H
