#include "udppacket.h"

udpPacket::udpPacket()
{
}

udpPacket::udpPacket(const udpPacket &u)
{
    mySender = u.mySender;
    myReceiver = u.myReceiver;
    myData = u.myData;
}

udpPacket& udpPacket::operator=(const udpPacket &u)
{
    mySender = u.mySender;
    myReceiver = u.myReceiver;
    myData = u.myData;
    return *this;
}
