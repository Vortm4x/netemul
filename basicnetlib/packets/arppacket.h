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
#ifndef ARPPACKET_H
#define ARPPACKET_H

#include "basicnetlib_global.h"
#include <QSharedData>
#include "macaddress.h"
#include "ipaddress.h"


class ArpPacketData : public QSharedData
{
public:
    ArpPacketData() { }
    ArpPacketData(const ArpPacketData &other);
    ~ArpPacketData() { }
    QByteArray toData() const;
    friend class ArpPacket;
private:
    quint8 type;
    MacAddress senderMac;
    MacAddress receiverMac;
    IpAddress senderIp;
    IpAddress receiverIp;
};

class BASICNETLIBSHARED_EXPORT ArpPacket
{
public:
    ArpPacket(MacAddress rm , MacAddress sm , IpAddress ri , IpAddress si , quint8 t);
    ArpPacket(const ArpPacket &other) : d(other.d) { }
    ArpPacket() { d = new ArpPacketData; }
    ArpPacket(const QByteArray &b);
    QByteArray toData() const;
    enum { request = 0 , response = 1 };
    void setType(int i) { d->type = i; }
    int type() const { return d->type; }
    void setSenderMac(MacAddress m) { d->senderMac = m; }
    void setSenderIp(IpAddress a) { d->senderIp = a; }
    void setReceiverMac(MacAddress m) { d->receiverMac = m; }
    void setReceiverIp(IpAddress a) { d->receiverIp = a; }
    MacAddress senderMac() const { return d->senderMac; }
    IpAddress senderIp() const { return d->senderIp; }
    MacAddress receiverMac() const  { return d->receiverMac; }
    IpAddress receiverIp() const { return d->receiverIp; }
private:
    QSharedDataPointer<ArpPacketData> d;
protected:
    friend QDataStream& operator<<(QDataStream &stream,const ArpPacket &p);
};
//-------------------------------------------------------------------
/*!
  Записывает arp-пакет в поток.
  @param stream - поток для записи.
  @param p - пакет для записи.
  @return обновленный поток.
*/
inline QDataStream& operator<<(QDataStream &stream,const ArpPacket &p)
{
    stream << p.toData();
    return stream;
}
//------------------------------------------------------------

#endif // ARPPACKET_H
