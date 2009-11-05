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

switchChip::switchChip(int c /* = 4 */ ) : boxChip(c)
{
    virtualNetwork *vlan = new virtualNetwork("VLAN1", this);
    myVlans << vlan;
    connect( this, SIGNAL(socketsCountChanged()), SLOT(checkPorts()) );
}

switchChip::~switchChip()
{
    myVlans.clear();
}

switchModel* switchChip::modelAt(virtualNetwork *vlan) const
{
    return vlan->table();
}

void switchChip::receiveEvent(frame &fr,devicePort *sender)
{
    checkReceive(fr);
    emit receiveData(fr,tr("LAN%1").arg(sender->num()));

    foreach ( virtualNetwork *i, myVlans )
        if ( i->containPort(sender) ) i->recieveEvent(fr, sender);
}

void switchChip::secondTimerEvent()
{
    foreach ( virtualNetwork *i, myVlans )
        i->secondTimerEvent();
}

void switchChip::sendDataSignal(frame &fr, QString port)
{
    emit sendData(fr, port);
}

void switchChip::checkPorts()
{
    if ( myVlans.count() > 1 ) return;
    myVlans.at(0)->includeAllPorts(sockets());
}


/****************************VLAN********************************/

virtualNetwork::virtualNetwork(const QString name,switchChip *chip)
{
    myTable = new switchModel;
    myName = name;
    mySwitchChip = chip;
    includeAllPorts(mySwitchChip->sockets());
}

virtualNetwork::~virtualNetwork()
{
    myDevicePorts.clear();
    delete myTable;
    delete mySwitchChip;
}

void virtualNetwork::recieveEvent(frame &fr, devicePort *sender)
{
    myTable->contains( fr.sender() , sender );

    devicePort *t = myTable->portWithMac( fr.receiver() );
    if ( t && t->isConnect() ) {
        mySwitchChip->checkSend(fr);
        mySwitchChip->sendDataSignal(fr, tr("LAN%1").arg(t->num()));
        t->pushToSend(fr);
        return;
    }
    foreach ( devicePort *i , myDevicePorts )
        if ( i != sender && i->isConnect() ) {
            mySwitchChip->checkSend(fr);
            mySwitchChip->sendDataSignal(fr, tr("LAN%1").arg(i->num()));
            i->pushToSend(fr);
        }
}

bool virtualNetwork::containPort(devicePort *port) const
{
    foreach ( devicePort *i, myDevicePorts )
        if ( i->num() == port->num() ) return true;
    return false;
}

void virtualNetwork::includeAllPorts(QStringList list)
{
    for ( int i = 0; i < list.size(); i++ )        
        if ( !myDevicePorts.contains(mySwitchChip->socket(list.at(i))) ) myDevicePorts << mySwitchChip->socket(list.at(i));
}

void virtualNetwork::secondTimerEvent()
{
    myTable->updateMac();
}

