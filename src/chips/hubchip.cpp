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
#include "hubchip.h"
#include "deviceport.h"

hubChip::hubChip(int n /* = 4 */) : boxChip(n)
{
    foreach ( devicePort *i , mySockets )
        i->setShared(true);
}

void hubChip::receiveEvent(frame &fr,devicePort *sender)
{
    checkReceive(fr);
    emit receiveData(fr,tr("LAN%1").arg(sender->num()));
    foreach ( devicePort *i , mySockets )
        if ( i != sender && i->isConnect() ) {
            checkSend(fr);
            emit sendData(fr, tr("LAN%1").arg(i->num()) );
            i->pushToSend( fr );
        }   
}

void hubChip::detectCollision()
{
    foreach ( devicePort *i , mySockets ) i->startCollision();
}

void hubChip::addSocket(int n)
{
    boxChip::addSocket(n);
    mySockets[n-1]->setShared(true);
}


