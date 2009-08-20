#include "tcppacket.h"

tcpPacket::tcpPacket(const QByteArray &b)
{
    d = new tcpPacketData;
    QDataStream s(b);
    s >> d->sender >> d->receiver >> d->sequence >> d->ack >> d->flag >> d->window >> d->data;
}

/*!
  Записывает tcp-сегмент в поток.
  @param stream - поток для записи.
  @param p - записываемый пакет.
  @return ссылку на результирующий поток.
*/
QDataStream& operator<<( QDataStream &stream, const tcpPacket &p )
{
    stream << p.toData();
    return stream;
}
//-------------------------------------------------------

tcpPacketData::tcpPacketData(const tcpPacketData &u) : QSharedData(u)
{
    sender = u.sender;
    receiver = u.receiver;
    sequence = u.sequence;
    ack = u.ack;
    flag = u.flag;
    window = u.window;
    data = u.data;
}

QByteArray tcpPacketData::toData() const
{
    QByteArray t;
    QDataStream s(&t,QIODevice::WriteOnly);
    s << receiver << sender << sequence << ack << flag << window << data;
    return t;
}
