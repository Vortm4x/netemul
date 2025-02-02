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
#ifndef DHCPPACKET_H
#define DHCPPACKET_H

#include "basicnetlib_global.h"
#include <QSharedData>
#include "ipaddress.h"
#include "macaddress.h"

class DhcpPacketData : public QSharedData
{
public:
    DhcpPacketData() { time = 0; }
    DhcpPacketData(const DhcpPacketData &other);
    ~DhcpPacketData() { }
    QByteArray toData() const;
    friend class DhcpPacket;
private:
    int xid;
    int type;
    int time;
    IpAddress mask;
    IpAddress gateway;
    IpAddress yiaddr; // proposed ip
    IpAddress siaddr; // server's ip
    MacAddress chaddr; // sender's mac-address
};

class BASICNETLIBSHARED_EXPORT DhcpPacket
{
public:
    DhcpPacket();
    DhcpPacket(const QByteArray &data);
    DhcpPacket(const DhcpPacket &other) : d(other.d) { }
    ~DhcpPacket() { }
    QByteArray toData() const;
    enum { DHCPDISCOVER , DHCPOFFER , DHCPREQUEST , DHCPACK , DHCPDECLINE };
    void setYiaddr(IpAddress a) { d->yiaddr = a; }
    void setSiaddr(IpAddress a) { d->siaddr = a; }
    void setMask(IpAddress a) { d->mask = a; }
    void setGateway(IpAddress a) { d->gateway = a; }
    void setChaddr(MacAddress a) { d->chaddr = a; }
    void setXid(int x) { d->xid = x; }
    void setType(int t) { d->type = t; }
    void setTime(int t) { d->time = t; }
    int type() const { return d->type; }
    int xid() const { return d->xid; }
    int time() const { return d->time; }
    IpAddress yiaddr() const { return d->yiaddr; }
    IpAddress siaddr() const { return d->siaddr; }
    IpAddress mask() const { return d->mask; }
    IpAddress gateway() const { return d->gateway; }
    MacAddress chaddr() const { return d->chaddr; }
    QString typeString() const;
private:
    QSharedDataPointer<DhcpPacketData> d;
};

#endif // DHCPPACKET_H
