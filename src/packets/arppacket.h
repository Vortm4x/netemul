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

#include <QSharedData>
#include "macaddress.h"
#include "ipaddress.h"


class arpPacketData : public QSharedData
{
public:
    arpPacketData() { }
    arpPacketData(const arpPacketData &other);
    ~arpPacketData() { }
    QByteArray toData() const;
    friend class arpPacket;
private:
    quint8 type;
    macAddress senderMac;
    macAddress receiverMac;
    ipAddress senderIp;
    ipAddress receiverIp;
};

class arpPacket
{
public:
    arpPacket(macAddress rm , macAddress sm , ipAddress ri , ipAddress si , quint8 t);
    arpPacket(const arpPacket &other) : d(other.d) { }
    arpPacket() { d = new arpPacketData; }
    arpPacket(const QByteArray &b);
    QByteArray toData() const;
    enum { request = 0 , response = 1 };
    void setType(int i) { d->type = i; }
    int type() const { return d->type; }
    void setSenderMac(macAddress m) { d->senderMac = m; }
    void setSenderIp(ipAddress a) { d->senderIp = a; }
    void setReceiverMac(macAddress m) { d->receiverMac = m; }
    void setReceiverIp(ipAddress a) { d->receiverIp = a; }
    macAddress senderMac() const { return d->senderMac; }
    ipAddress senderIp() const { return d->senderIp; }
    macAddress receiverMac() const  { return d->receiverMac; }
    ipAddress receiverIp() const { return d->receiverIp; }
private:
    QSharedDataPointer<arpPacketData> d;
protected:
    friend QDataStream& operator<<(QDataStream &stream,const arpPacket &p);
};
//-------------------------------------------------------------------
/*!
  Записывает arp-пакет в поток.
  @param stream - поток для записи.
  @param p - пакет для записи.
  @return обновленный поток.
*/
inline QDataStream& operator<<(QDataStream &stream,const arpPacket &p)
{
    stream << p.toData();
    return stream;
}
//------------------------------------------------------------

#endif // ARPPACKET_H
