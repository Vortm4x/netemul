#include "arppacket.h"

arpPacket::arpPacket()
{
}

arpPacket arpPacket::operator=(arpPacket other)
{
    mySenderMac = other.mySenderMac;
    myReceiverMac = other.myReceiverMac;
    mySenderIp = other.mySenderIp;
    myReceiverIp = other.myReceiverIp;
    myType = other.myType;
    return *this;
}
