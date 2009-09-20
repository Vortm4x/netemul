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
#ifndef DHCPSERVERPROGRAMM_H
#define DHCPSERVERPROGRAMM_H

#include "programmrep.h"
#include "macaddress.h"
#include "ipaddress.h"

class udpSocket;

struct staticRecord {
    macAddress chaddr;
    ipAddress yiaddr;
    ipAddress mask;
    ipAddress gateway;
    int time;
};

class dhcpServerProgramm : public programmRep
{
    Q_OBJECT
public:
    enum { DHCPServer = 52 , CLIENT_SOCKET = 67 , SERVER_SOCKET = 68 };
    dhcpServerProgramm();
    ~dhcpServerProgramm() { }
    void setDevice(smartDevice *s);
    void showProperty();
    bool containRecord( staticRecord *rec);
    void addStaticRecord(staticRecord *rec);
    void incTime() { }
    bool interrupt(int) { return false; }
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
public slots:
    void execute(QByteArray data);
private:
    QList<staticRecord*> statics;
    udpSocket *receiver;
};

#endif // DHCPSERVERPROGRAMM_H
