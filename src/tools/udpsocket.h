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
#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include "abstractsocket.h"
#include <QQueue>
#include <QByteArray>

class udpSocket : public abstractSocket
{
    Q_OBJECT
public:
    enum { PACKET_SIZE = 1024 };
    udpSocket() { }
    udpSocket(SmartDevice *d, quint16 port);
    ~udpSocket() { }
    void treatPacket(ipPacket p);
    void write(IpAddress address, quint16 port, QByteArray data);
private:
    void writeBroadcast(quint16 port, QByteArray data);
};

#endif // UDPSOCKET_H
