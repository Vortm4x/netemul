#include "udppacket.h"

udpPacket::udpPacket()
{
}

QByteArray udpPacket::toData() const
{
    QByteArray t;
    QDataStream s(&t,QIODevice::WriteOnly);
    s << myReceiver << mySender << data;
    return t;
}

udpPacket::udpPacket(const QByteArray &b)
{
    QDataStream s(b);
    s >> myReceiver >> mySender >> data;
}

udpPacket::udpPacket(const udpPacket &u)
{
    mySender = u.mySender;
    myReceiver = u.myReceiver;
    data = u.data;
}

udpPacket& udpPacket::operator=(const udpPacket &u)
{
    mySender = u.mySender;
    myReceiver = u.myReceiver;
    data = u.data;
    return *this;
}
