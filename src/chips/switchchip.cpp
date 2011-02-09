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
#include "switchchip.h"
#include "deviceport.h"
#include "switchmodel.h"
#include <QStringList>

#include <iostream>

SwitchChip::SwitchChip(QObject *parent) : BoxChip(parent)
{
    connect( this, SIGNAL(socketsCountChanged()), SLOT(checkPorts()) );
}

SwitchChip* SwitchChip::create(QObject *parent)
{
    SwitchChip *chip = new SwitchChip(parent);
    VirtualNetwork *vlan = new VirtualNetwork(chip);
    vlan->setName("VLAN1");
    chip->addVirtualNetwork(vlan);
    return chip;
}

SwitchChip::~SwitchChip()
{
    myVlans.clear();
}

SwitchModel* SwitchChip::modelAt(VirtualNetwork *vlan) const
{
    return vlan->table();
}

void SwitchChip::receiveEvent(Frame &fr,DevicePort *sender)
{
    checkReceive(fr);
    emit receiveData(fr,tr("LAN%1").arg(sender->num()));

    foreach ( VirtualNetwork *i, myVlans )
        if ( i->containPort(sender) ) i->recieveEvent(fr, sender);
}

void SwitchChip::secondTimerEvent()
{
    foreach ( VirtualNetwork *i, myVlans )
        i->secondTimerEvent();
}

void SwitchChip::sendDataSignal(Frame &fr, QString port)
{
    emit sendData(fr, port);
}

void SwitchChip::checkPorts()
{
    if ( myVlans.count() == 1 ) {
        myVlans.at(0)->includeAllPorts(sockets());
    }
}

void SwitchChip::addVirtualNetwork(VirtualNetwork *vlan)
{
    myVlans << vlan;
    if ( myVlans.count() == 1 ) {
        myVlans.at(0)->includeAllPorts(sockets());
    }
}


/****************************VLAN********************************/

VirtualNetwork::VirtualNetwork(QObject *parent) : QObject(parent)
{
    myTable = new SwitchModel;
    mySwitchChip = qobject_cast<SwitchChip*>(parent);
    includeAllPorts(mySwitchChip->sockets());
}

VirtualNetwork::~VirtualNetwork()
{
    myDevicePorts.clear();
    delete myTable;    
}

void VirtualNetwork::recieveEvent(Frame &fr, DevicePort *sender)
{
    myTable->contains( fr.sender() , mySwitchChip->portToString(sender) );

    QString str = myTable->portWithMac( fr.receiver() );
    if ( !str.isEmpty() ) {
        DevicePort *t = mySwitchChip->findPortByName(str);
        if ( t && t->isConnect() ) {
            mySwitchChip->checkSend(fr);
            mySwitchChip->sendDataSignal(fr, tr("LAN%1").arg(t->num()));
            t->pushToSend(fr);
            return;
        }
    }
    foreach ( DevicePort *i , myDevicePorts )
        if ( i != sender && i->isConnect() ) {
            mySwitchChip->checkSend(fr);
            mySwitchChip->sendDataSignal(fr, tr("LAN%1").arg(i->num()));
            i->pushToSend(fr);
        }
}

bool VirtualNetwork::containPort(DevicePort *port) const
{
    foreach ( DevicePort *i, myDevicePorts )
        if ( i->num() == port->num() ) return true;
    return false;
}

void VirtualNetwork::includeAllPorts(QStringList list)
{
    for ( int i = 0; i < list.size(); i++ )        
        if ( !myDevicePorts.contains(mySwitchChip->socket(list.at(i))) )
            myDevicePorts << mySwitchChip->socket(list.at(i));
}

QStringList VirtualNetwork::devicePorts() const
{
    QStringList list;
    foreach ( DevicePort *i, myDevicePorts )
        list << tr("LAN%1").arg(i->num()) ;
    return list;
}

void VirtualNetwork::secondTimerEvent()
{
    myTable->updateMac();
}

QVariantList VirtualNetwork::recordList() const
{
    return myTable->recordList();
}

void VirtualNetwork::addMacRecordObject(MacRecordObject *obj)
{
    myTable->addToTable( obj->record() );
    obj->deleteLater();
}

