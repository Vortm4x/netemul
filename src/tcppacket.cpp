/****************************************************************************************
** NetEmul - program for simulating computer networks.
** Copyright © 2009 Semenov Pavel and Omilaeva Anastasia
**
** NetEmul is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** NetEmul is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with the NetEmul; if not, write to the Free
** Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
** 02111-1307 USA.
****************************************************************************************/
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
