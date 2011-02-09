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
#ifndef IPPACKET_H
#define IPPACKET_H

#include "basicnetlib_global.h"
#include <QSharedData>
#include "ipaddress.h"

/*!
  * Содержит в себе разделяемые между ip пакетами данные.
*/
class IpPacketData : public QSharedData
{
public:
    IpPacketData() {  }
    IpPacketData(const IpPacketData &other);
    ~IpPacketData() {  }
    friend class IpPacket;
private:
    IpAddress sender; //!< Адрес отправителя.
    IpAddress receiver; //!< Адрес получателя.
    qint8 upProtocol; //!< Протокол верхнего уровня
    quint8 ttl;
    QByteArray data; //!< Данные протокола более высокого уровня.
    QByteArray toData() const;
};


/*!
  Реализует ip-пакет, так же как и в реальной сети содержит адрес отправителя, получателя,
  тип протокола верхнего уровня и поле данных. Остальные папраметры реального пакета пока
  не используются.
*/
class BASICNETLIBSHARED_EXPORT IpPacket
{
public:
    /*! Используется для обозначения протокола верхнего уровня. */
    enum { udp = 0 , tcp = 1 };
    IpPacket() { d = new IpPacketData; d->ttl = 64; }
    IpPacket(const QByteArray &b);
    IpPacket(IpAddress s,IpAddress r);
    ~IpPacket() { }
    IpPacket(const IpPacket &other) : d(other.d) { }
    QByteArray toData() const;
    QString toString() const;

    IpAddress sender() const { return d->sender; }
    IpAddress receiver() const { return d->receiver; }
    quint8 ttl() const { return d->ttl; }
    void setSender(IpAddress a) { d->sender = a; }
    void setReceiver(IpAddress a) { d->receiver = a; }
    void setTtl(quint8 ttl) { d->ttl = ttl; }
    bool isBroadcast(const IpAddress mask) const;
    quint16 receiverSocket() const;
    quint8 decTtl();
    void setBroadcast(const IpAddress mask);
    void setUpProtocol(qint8 u) { d->upProtocol = u; }
    qint8 upProtocol() const { return d->upProtocol; }
    void pack(const QByteArray &b) { d->data = b; }
    int size() { return d->data.size(); }
    QByteArray unpack() const  { return d->data; }
private:
    QSharedDataPointer<IpPacketData> d; //!< Данные пакета.
protected:
    friend bool operator==(const IpPacket &p1,const IpPacket &p2);
    friend QDataStream& operator<<(QDataStream &stream,const IpPacket &p);
};
//---------------------------------------------------------------------------
/*!
  Записывает ip-пакет в поток.
  @param stream - поток для записи.
  @param p - записываемый пакет.
  @return ссылку на результирующий поток.
*/
inline QDataStream& operator<<(QDataStream &stream,const IpPacket &p)
{
    stream << p.toData();
    return stream;
}
//--------------------------------------------------
/*!
  Назначает пакету широковещательный адрес исходя из маски.
  @param mask - Маска.
*/
inline void IpPacket::setBroadcast(const IpAddress mask)
{
    d->receiver = d->sender | ~mask;
}
//---------------------------------------------------
inline bool operator==(const IpPacket &p1,const IpPacket &p2)
{
    return ( p1.sender() == p2.sender() && p1.receiver() == p2.receiver() );
}

#endif // IPPACKET_H
