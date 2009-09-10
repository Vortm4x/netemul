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

class switchModel;

class switchChip : public boxChip
{
    Q_OBJECT
public:
    switchChip(int c = 4);
    ~switchChip();
    void receiveEvent(frame &fr,devicePort *sender);
    void deleteFromTable(macAddress mac);
    void dialog();
    void secondTimerEvent();
    switchModel* model() { return switchTable; }
private:
    switchModel *switchTable;
};

#endif // SWITCHCHIP_H
