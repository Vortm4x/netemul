#include "arppacket.h"

arpPacket::arpPacket(macAddress rm , macAddress sm , ipAddress ri , ipAddress si , quint8 t)
{
    myReceiverMac = rm;
    mySenderMac = sm;
    myReceiverIp = ri;
    mySenderIp = si;
    myType = t;
}

arpPacket::arpPacket(const QByteArray &b)
{
    QDataStream s(b);
    s >> myType >> mySenderMac >> myReceiverMac >> mySenderIp >> myReceiverIp;
}

QByteArray arpPacket::toData() const
{
    QByteArray t;
    QDataStream s(&t,QIODevice::WriteOnly);
    s << myType << mySenderMac << myReceiverMac << mySenderIp << myReceiverIp;
    return t;
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

