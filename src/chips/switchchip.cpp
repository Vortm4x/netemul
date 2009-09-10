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

switchChip::switchChip(int c /* = 4 */ ) : boxChip(c)
{
    switchTable = new switchModel;
}

switchChip::~switchChip()
{
    delete switchTable;
}

void switchChip::receiveEvent(frame &fr,devicePort *sender)
{
    checkReceive(fr);
    emit receiveData(fr,tr("LAN%1").arg(sender->num()));

    switchTable->contains( fr.sender() , sender );

    devicePort *t = switchTable->portWithMac( fr.receiver() );
    if ( t && t->isConnect() ) {
        checkSend(fr);
        emit sendData(fr, tr("LAN%1").arg(t->num()));
        t->pushToSend(fr);
        return;
    }
    foreach ( devicePort *i , mySockets ) {
        if ( i != sender && i->isConnect() ) {
            checkSend(fr);
            emit sendData(fr, tr("LAN%1").arg(i->num()));
            i->pushToSend(fr);
        }
    }
}

void switchChip::secondTimerEvent()
{
    switchTable->updateMac();
}


