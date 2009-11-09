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
#ifndef INTERFACE_H
#define INTERFACE_H

#include "abstractchip.h"
#include "ippacket.h"
#include "arppacket.h"

class cableDev;
class arpModel;

static const quint8 COUNT_AGAINST_SEND = 5;

struct waitPacket
{
    ipAddress dest;
    int time;
    quint8 count;
    QList<ipPacket> packets;
    void insert(ipPacket p) { packets << p; }
    static waitPacket* create(ipAddress a,ipPacket p);
};

class interface : public abstractChip
{
    Q_OBJECT
public:
    interface(const QString &name);
    interface() { }
    ~interface();
    void receiveEvent(frame &fr,devicePort*);
    void receiveIp(ipPacket &ip);
    void receiveArp(arpPacket &arp);
    void sendPacket(ipPacket &p,ipAddress gw = ipAddress("0.0.0.0"));
    void sendBroadcast(ipPacket &p);
    const devicePort* socket() const { return mySocket; }
    bool isConnect() const;
    void setConnect(bool b,cableDev *c);
    bool isCableConnect(const cableDev *c) const;
    void deciSecondEvent();
    void secondEvent();
    void sendArpRequest(ipAddress a);
    void sendArpResponse(macAddress m, ipAddress a);
    int trafficDigit() const;
    bool isBusy() const;

    frame createFrame( macAddress receiverMac , int t);
    void setChecked(bool b);
    arpModel* arpTable() const { return myArpTable; }

    void pushToSocket(frame &f);

    virtual void write(QDataStream &stream) const;
    virtual void read(QDataStream &stream);
    virtual void writeXml(sceneXmlWriter &stream) const;
    virtual void readXml(sceneXmlReader &stream);
    void setName(const QString &str) { myName = str; }
    QString name() const { return myName; }
signals:
    void receivedPacket(ipPacket);
    void equalIpDetected();
private:
    QString myName;
    devicePort *mySocket;
    arpModel *myArpTable;
    QList<waitPacket*> myWaits;
};

#endif // INTERFACE_H
