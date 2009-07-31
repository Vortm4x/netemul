#include "arppacket.h"

arpPacket::arpPacket(macAddress rm , macAddress sm , ipAddress ri , ipAddress si , quint8 t)
{
    d = new arpPacketData;
    d->receiverMac = rm;
    d->senderMac = sm;
    d->receiverIp = ri;
    d->senderIp = si;
    d->type = t;
}

arpPacket::arpPacket(const QByteArray &b)
{
    d = new arpPacketData;
    QDataStream s(b);
    s >> d->type >> d->senderMac >> d->receiverMac >> d->senderIp >> d->receiverIp;
}

QByteArray arpPacket::toData() const
{
    return d->toData();
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

QByteArray arpPacketData::toData() const
{
    QByteArray t;
    QDataStream s(&t,QIODevice::WriteOnly);
    s << type << senderMac << receiverMac << senderIp << receiverIp;
    return t;
}

arpPacketData::arpPacketData(const arpPacketData &other) : QSharedData(other)
{
    receiverMac = other.receiverMac;
    senderMac = other.senderMac;
    receiverIp = other.receiverIp;
    senderIp = other.senderIp;
    type = other.type;
}

