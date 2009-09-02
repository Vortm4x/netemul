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

QString udpPacket::typeToString() const
{
    switch ( d->receiver ) {
        case User: return QObject::tr("UDP Message user");
        case RIP: return QObject::tr("RIP");
        case DHCPClient: case DHCPServer: return QObject::tr("DHCP message");
        default: return QObject::tr("None");
    }
}

QString udpPacket::toString() const
{
    QString temp;
    temp.append("UDP, "+QObject::tr("sender port: %1, receiver port: %2").arg(d->sender).arg(d->receiver) );
    return temp;
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
