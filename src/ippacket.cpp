#include "ippacket.h"

ipPacket::ipPacket()
{
    data.clear();
}

ipPacket ipPacket::operator=(ipPacket other)
{
    mySender = other.mySender;
    myReceiver = other.myReceiver;
    data = other.data;
    return *this;
}

void ipPacket::operator<<(const ipAddress &a)
{
    QDataStream in(&data,QIODevice::WriteOnly);
    in << a;
}

void ipPacket::operator<<(const QVariant s)
{
    QDataStream in(&data,QIODevice::WriteOnly);
    in << s;
}

void ipPacket::operator>>(QVariant &s) const
{
    QDataStream out(data);
    out >> s;
}

bool ipPacket::isBroadcast(const ipAddress mask) const
{
    ipAddress a = ~mask;
    if ( ( myReceiver & a ) == a) return true;
    return false;
}

void ipPacket::setBroadcast(const ipAddress mask)
{
    myReceiver = mySender | ~mask;
}
