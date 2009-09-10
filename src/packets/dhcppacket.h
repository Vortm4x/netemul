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

#include "ipaddress.h"
#include "macaddress.h"

class dhcpPacket
{
public:
    dhcpPacket();
    void setYiaddr(ipAddress a) { myYiaddr = a; }
    void setSiaddr(ipAddress a) { mySiaddr = a; }
    void setChaddr(macAddress a) { myChaddr = a; }
    void setXid(int x) { myXid = x; }
    int xid() const { return myXid; }
    ipAddress yiaddr() const { return myYiaddr; }
    ipAddress siaddr() const { return mySiaddr; }
    macAddress chaddr() const { return myChaddr; }
private:
    int myXid;
    ipAddress myYiaddr;
    ipAddress mySiaddr;
    macAddress myChaddr;
};

#endif // DHCPPACKET_H
