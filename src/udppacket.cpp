#include "udppacket.h"

QByteArray udpPacket::toData() const
{
    return d->toData();
}

udpPacket::udpPacket(const QByteArray &b)
{
    d = new udpPacketData;
    QDataStream s(b);
    s >> d->receiver >> d->sender >> d->data;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

udpPacketData::udpPacketData(const udpPacketData &u) : QSharedData(u)
{
    sender = u.sender;
    receiver = u.receiver;
    data = u.data;
}

QByteArray udpPacketData::toData() const
{
    QByteArray t;
    QDataStream s(&t,QIODevice::WriteOnly);
    s << receiver << sender << data;
    return t;
}
