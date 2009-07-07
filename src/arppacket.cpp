#include "arppacket.h"

arpPacket::arpPacket(macAddress rm , macAddress sm , ipAddress ri , ipAddress si , quint8 t)
{
    myReceiverMac = rm;
    mySenderMac = sm;
    myReceiverIp = ri;
    mySenderIp = si;
    myType = t;
}

arpPacket& arpPacket::operator=(arpPacket &other)
{
    mySenderMac = other.mySenderMac;
    myReceiverMac = other.myReceiverMac;
    mySenderIp = other.mySenderIp;
    myReceiverIp = other.myReceiverIp;
    myType = other.myType;
    return *this;
}

arpPacket::arpPacket(const arpPacket &other)
{
    mySenderMac = other.mySenderMac;
    myReceiverMac = other.myReceiverMac;
    mySenderIp = other.mySenderIp;
    myReceiverIp = other.myReceiverIp;
    myType = other.myType;
}

