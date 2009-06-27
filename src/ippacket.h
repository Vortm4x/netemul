#ifndef IPPACKET_H
#define IPPACKET_H

#include "ipaddress.h"
#include <QVariant>

class ipPacket
{
public:
    ipPacket();
    ipPacket(ipAddress s,ipAddress r) { mySender = s ; myReceiver = r; }
    ipAddress sender() const { return mySender; }
    ipAddress receiver() const { return myReceiver; }
    void setSender(ipAddress a) { mySender = a; }
    void setReceiver(ipAddress a) { myReceiver = a; }
    bool isBroadcast(const ipAddress mask) const;
    bool isBroadcast(const QString str) const { return isBroadcast(ipAddress(str)); }
    void setBroadcast(const ipAddress mask);
    void setBroadcast(const QString str) { setBroadcast(ipAddress(str)); }
    ipPacket operator=(ipPacket other);
    void operator<<(const ipAddress &a);
    void operator<<(const QVariant s);
    void operator>>(QVariant &s) const;
private:
    ipAddress mySender;
    ipAddress myReceiver;
    QByteArray data;
};

#endif // IPPACKET_H
