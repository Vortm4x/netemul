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
#ifndef SWITCHCHIP_H
#define SWITCHCHIP_H

#include "boxchip.h"

class SwitchModel;
class VirtualNetwork;
class MacRecordObject;

class SwitchChip : public BoxChip
{
    Q_OBJECT
public:
    SwitchChip(QObject *parent);
    static SwitchChip* create(QObject *parent);
    ~SwitchChip();
    void receiveEvent(Frame &fr,DevicePort *sender);
    void dialog();
    void secondTimerEvent();
    void sendDataSignal(Frame &fr, QString port);
    SwitchModel* modelAt(VirtualNetwork *vlan) const;
    VirtualNetwork* vlanAt(int n) const { return myVlans.at(n); }
    Q_INVOKABLE void addVirtualNetwork(VirtualNetwork *vlan);
public slots:
    void checkPorts();
private:
    QList<VirtualNetwork*> myVlans;
};


// VLAN

class VirtualNetwork : public QObject
{
    Q_OBJECT
    Q_PROPERTY( QString name READ name WRITE setName )
    Q_PROPERTY( QVariantList recordList READ recordList )
// Инициализация
public:
    VirtualNetwork(QObject *parent = 0);
    ~VirtualNetwork();
public:
    void recieveEvent(Frame&fr, DevicePort *sender);
    void secondTimerEvent();
// � абота со списком
public:
    bool containPort(DevicePort *port) const;
    void includeAllPorts(QStringList list);
    Q_INVOKABLE void addMacRecordObject(MacRecordObject *obj);
// Атрибуты
public:
    void setName(QString n) { myName = n; }
    QString name() const { return myName; }
    SwitchModel* table() const { return myTable; }
    QStringList devicePorts() const;
    QVariantList recordList() const;
private:
    QList<DevicePort*> myDevicePorts;
    QString myName;
    SwitchChip *mySwitchChip;
    SwitchModel *myTable;
};

#endif // SWITCHCHIP_H
