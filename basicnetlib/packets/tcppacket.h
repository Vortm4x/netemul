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
#ifndef TCPPACKET_H
#define TCPPACKET_H

#include <QDataStream>
#include <QSharedData>
#include "basicnetlib_global.h"

class tcpPacketData : public QSharedData
{
public:
    tcpPacketData() { }
    tcpPacketData(const tcpPacketData &other);
    ~tcpPacketData() { }
    QByteArray toData() const;
    friend class tcpPacket;
private:
    quint16 sender; //!< Порт отправителя
    quint16 receiver; //!< Порт получателя
    quint32 sequence;
    quint32 ack;
    quint8 flag;
    quint16 window;
    QByteArray data; //!< Поле данных
};

/*!
  Реализует tcp-сегмент
*/
class BASICNETLIBSHARED_EXPORT tcpPacket
{
public:
    enum { User = 7777 , Window = 10240 };
    enum { NO_FLAGS = 0, SYN = 1, ACK = 2, FIN = 4, RST = 8 };
    tcpPacket() { d = new tcpPacketData; }
    tcpPacket(const QByteArray &b);
    tcpPacket(const tcpPacket &other) : d(other.d) { }
    ~tcpPacket() { }
    QByteArray toData() const { return d->toData(); }
    int size() const { return d->data.size(); }
    void setSender(quint16 s) { d->sender = s; }
    void setReceiver(quint16 r) { d->receiver = r; }
    void setSequence(quint32 s) { d->sequence = s; }
    void setAck(quint32 a) { d->ack = a; }
    void setFlag(quint8 f) { d->flag = f; }
    void setWindow(quint16 w) { d->window = w; }
    quint16 sender() const { return d->sender; }
    quint16 receiver() const { return d->receiver; }
    quint32 sequence() const { return d->sequence; }
    quint32 ack() const { return d->ack; }
    quint8 flag() const { return d->flag; }
    quint16 window() const { return d->window; }
    QByteArray unpack() const { return d->data; }
    void pack(const QByteArray &b) { d->data = b; }
    QString toString() const;
private:
    QSharedDataPointer<tcpPacketData> d;
protected:    
    friend QDataStream& operator<<( QDataStream &stream, const tcpPacket &p );
};

#endif // TCPPACKET_H
