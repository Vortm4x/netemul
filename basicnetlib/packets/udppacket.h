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
#ifndef UDPPACKET_H
#define UDPPACKET_H

#include "basicnetlib_global.h"
#include <QDataStream>
#include <QSharedData>

class UdpPacketData : public QSharedData
{
public:
    UdpPacketData() { }
    UdpPacketData(const UdpPacketData &other);
    ~UdpPacketData() { }
    QByteArray toData() const;
    friend class UdpPacket;
private:
    quint16 sender; //!< Порт отправителя
    quint16 receiver; //!< Порт получателя
    QByteArray data; //!< Поле данных
};
/*!
  Реализует udp-дейтаграмму
*/
class BASICNETLIBSHARED_EXPORT UdpPacket
{
public:
    enum { User = 7777 , RIP = 520 , DHCPClient = 67, DHCPServer = 68 } ;
    UdpPacket() { d = new UdpPacketData; }
    UdpPacket(const QByteArray &b);
    UdpPacket(const UdpPacket &u) : d(u.d) { }
    ~UdpPacket() { }
    QByteArray toData() const;
    int size() const { return d->data.size(); }
    void setSender( quint16 i ) { d->sender = i;}
    void setReceiver( quint16 i ) { d->receiver = i; }
    quint16 sender() const { return d->sender; }
    quint16 receiver() const { return d->receiver; }
    void pack(const QByteArray &b) { d->data = b; }
    QByteArray unpack() const { return d->data; }
    QString typeToString() const;
    QString toString() const;
private:
    QSharedDataPointer<UdpPacketData> d;
protected:
    friend QDataStream& operator<<( QDataStream &stream, const UdpPacket &p );
};
//-----------------------------------------------------
/*!
  Записывает udp-дейтаграмму в поток.
  @param stream - поток для записи.
  @param p - записываемый пакет.
  @return ссылку на результирующий поток.
*/
inline QDataStream& operator<<( QDataStream &stream, const UdpPacket &p )
{
    stream << p.toData();
    return stream;
}
//-------------------------------------------------------
#endif // UDPPACKET_H
