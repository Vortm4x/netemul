#include "tcppacket.h"

tcpPacket::tcpPacket(const QByteArray &b)
{
    d = new tcpPacketData;
    QDataStream s(b);
    s >> d->receiver >> d->sender >> d->sequence >> d->ack >> d->flag >> d->window >> d->data;
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

QString tcpPacket::toString() const
{
    QString temp;
    temp.append("TCP, "+QObject::tr("sender port: %1, receiver port: %2").arg(d->sender).arg(d->receiver) );
    return temp;
}

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
