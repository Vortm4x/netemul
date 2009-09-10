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
#ifndef DHCPCLIENTPROGRAMM_H
#define DHCPCLIENTPROGRAMM_H

#include "programmrep.h"

static const int FIVE_MINUTE = 300;

class dhcpClientProgramm : public programmRep
{
    Q_OBJECT
public:
    enum { DHCPClient = 51 ,CLIENT_SOCKET = 67, SERVER_SOCKET = 68  };
    enum state { NONE = 0 , WAIT_VARIANT = 1 , WAIT_RESPONCE = 2 , ALL_RIGHT = 3 };
    dhcpClientProgramm();
    ~dhcpClientProgramm() { }
    bool interrupt(int) { return false; }
    void showProperty();
    void incTime();
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
private:
    int xid;
    state myState;
    void sendDiscover();
    int time;
};

#endif // DHCPCLIENTPROGRAMM_H
