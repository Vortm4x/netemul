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
#include "dhcpclientprogramm.h"
#include "smartdevice.h"
#include "udpsocket.h"
#include "dhcppacket.h"
#include "dhcpclientproperty.h"

dhcpClientProgramm::dhcpClientProgramm()
{
    myName = tr("DHCP client");
    myState = NONE;
    time = 0;
}

void dhcpClientProgramm::incTime()
{
    time++;
    if (  myState == NONE && time%FIVE_MINUTE == 0 ) sendDiscover();
}

void dhcpClientProgramm::sendDiscover()
{
    xid = qrand()%5000;
    dhcpPacket p;
    p.setXid(xid);
    QByteArray data;
    udpSocket socket(sd, CLIENT_SOCKET);
    socket.write( ipAddress::full() , SERVER_SOCKET , data);
}

void dhcpClientProgramm::showProperty()
{
    dhcpClientProperty *d = new dhcpClientProperty;
    d->exec();
}

/*!
  Записывает отличительные черты в поток.
  @param stream - поток для записи.
*/
void dhcpClientProgramm::write(QDataStream &stream) const
{
    stream << DHCPClient;
    programmRep::write(stream);
}
//---------------------------------------------------
/*!
  Считывает отличительные черты из потока.
  @param stream - поток для чтения.
*/
void dhcpClientProgramm::read(QDataStream &stream)
{
    programmRep::read(stream);
}
//---------------------------------------------------
