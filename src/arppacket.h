#ifndef ARPPACKET_H
#define ARPPACKET_H

#include "macaddress.h"
#include "ipaddress.h"

class arpPacket
{
public:
    arpPacket(macAddress rm , macAddress sm , ipAddress ri , ipAddress si , int t);
    arpPacket() { }
    enum { request = 0 , answer = 1 };
    void setType(int i) { myType = i; }
    int type() const { return myType; }
    void setSenderMac(macAddress m) { mySenderMac = m; }
    void setSenderIp(ipAddress a) { mySenderIp = a; }
    void setReceiverMac(macAddress m) { myReceiverMac = m; }
    void setReceiverIp(ipAddress a) { myReceiverIp = a; }
    macAddress senderMac() const { return mySenderMac; }
    ipAddress senderIp() const { return mySenderIp; }
    macAddress receiverMac() const  { return myReceiverMac; }
    ipAddress receiverIp() const { return myReceiverIp; }
    arpPacket operator=(arpPacket other);
private:
    int myType;
    macAddress mySenderMac;
    macAddress myReceiverMac;
    ipAddress mySenderIp;
    ipAddress myReceiverIp;
protected:
    friend QDataStream& operator<<(QDataStream &stream,const arpPacket &p);
    friend QDataStream& operator>>(QDataStream &stream,arpPacket &p);
};

#endif // ARPPACKET_H
