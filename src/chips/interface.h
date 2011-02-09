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

class Cable;
class ArpModel;
class Interface;

typedef QVector<Interface*> InterfaceVector;

static const quint8 COUNT_AGAINST_SEND = 5;

struct waitPacket
{
    IpAddress dest;
    int time;
    quint8 count;
    QList<IpPacket> packets;
    void insert(IpPacket p) { packets << p; }
    static waitPacket* create(IpAddress a,IpPacket p);
};

class Interface : public AbstractChip
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName )
public:    
    Interface(QObject *parent = 0);
    static Interface* create(QObject *parent = 0);
    ~Interface();
    void receiveEvent(Frame &fr,DevicePort*);
    void receiveIp(IpPacket &ip);
    void receiveArp(ArpPacket &arp);
    void sendPacket(IpPacket &p,IpAddress gw = IpAddress("0.0.0.0"));
    void sendBroadcast(IpPacket &p);
    DevicePort* socket() const { return mySocket; }
    Cable* socketCable() const;
    bool isConnect() const;    
    bool isCableConnect(const Cable *c) const;    
    void deciSecondEvent();
    void secondEvent();
    void sendArpRequest(IpAddress a);
    void sendArpResponse(MacAddress m, IpAddress a);
    int trafficDigit() const;
    bool isBusy() const;    

    Frame createFrame( MacAddress receiverMac , int t);
    void setChecked(bool b);
    ArpModel* arpTable() const { return myArpTable; }
    Q_INVOKABLE void setArpModel(ArpModel *model) {
        if ( model ) {
            myArpTable = model;
        }
    }

    void pushToSocket(Frame &f);

    virtual void write(QDataStream &stream) const;
    virtual void read(QDataStream &stream);
    void setName(const QString &str) { myName = str; }
    QString name() const { return myName; }

public slots:
    //void onCableConnect(Cable *c);

signals:
    void cableConnected(Cable*);
    void receivedPacket(IpPacket);
    void equalIpDetected();
private:
    QString myName;
    DevicePort *mySocket;
    ArpModel *myArpTable;
    QList<waitPacket*> myWaits;
};

#endif // INTERFACE_H
