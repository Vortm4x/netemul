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
#include "ipaddress.h"
#include "macaddress.h"

class udpSocket;
class dhcpPacket;
class interface;
class staticDhcpRecord;
class dhcpServerModel;


struct clientState {
    int xid;
    int state;
    macAddress mac;
    ipAddress ip;
    ipAddress mask;
    ipAddress gateway;
    int time;
    int requestTimer;
    enum { WAIT_REQUEST = 0, IN_USE = 1 };
    clientState(staticDhcpRecord *rec);
    clientState() { }
};

class dhcpServerProgramm : public programmRep
{
    Q_OBJECT
public:
    enum { DHCPServer = 2 , CLIENT_SOCKET = 67 , SERVER_SOCKET = 68 };
    dhcpServerProgramm();
    ~dhcpServerProgramm();
    int id() const { return DHCPServer; }
    void setDevice(smartDevice *s);
    void setInterface( QString inter );
    void setBegin(ipAddress ip) { myBeginIp = ip; }
    void setEnd(ipAddress ip) { myEndIp = ip; }
    void setMask(ipAddress m) { myMask = m; }
    void setGateway(ipAddress g) { myGateway = g; }
    void setTime(int t) { myTime = t; }
    void setWaitingTime(int t) { myWaitingTime = t; }
    void setDynamic(bool b) { myDynamic = b; }   
    ipAddress beginIp() const { return myBeginIp; }
    ipAddress endIp() const { return myEndIp; }
    ipAddress mask() const { return myMask; }
    ipAddress gateway() const { return myGateway; }
    QString interfaceName() const { return myInterface; }
    int time() const { return myTime; }
    int waitingTime() const { return myWaitingTime; }
    bool dynamic() const { return myDynamic; }
    void showProperty();
    dhcpServerModel* dhcpModel() { return myDhcpModel; }
    clientState* findClient( int xid ) const;
    ipAddress giveDynamicIp() const;
    void incTime();
    bool interrupt(int) { return false; }
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
public slots:
    void execute(QByteArray data);
private:
    void executeDiscover(dhcpPacket packet);
    void executeRequest(dhcpPacket packet);
    void sendDhcp(dhcpPacket packet) const;
    dhcpPacket buildOffer(staticDhcpRecord *rec, int id);
    dhcpPacket createDhcpPacket( clientState *client, int state ) const;
    clientState* chooseDynamic(dhcpPacket packet);
    bool containClient(ipAddress ip) const;
    QList<clientState*> clients;
    QString myInterface;
    udpSocket *receiver;
    dhcpServerModel *myDhcpModel;
    ipAddress myBeginIp;
    ipAddress myEndIp;
    ipAddress myMask;
    ipAddress myGateway;
    int myTime;
    int myWaitingTime;
    bool myDynamic;
};

#endif // DHCPSERVERPROGRAMM_H
