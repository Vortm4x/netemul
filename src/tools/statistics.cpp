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
#include <QRegExp>
#include "statistics.h"

Statistics::Statistics()
{
    clear();
}

void Statistics::clear()
{
    receiveFrames = 0 ;
    receivePackets = 0 ;
    sendFrames = 0;
    sendPackets = 0;
}

Statistics::Statistics(const Statistics &other)
{
    receiveFrames = other.receiveFrames ;
    receivePackets = other.receivePackets ;
    sendFrames = other.sendFrames ;
    sendPackets = other.sendPackets ;
}

Statistics Statistics::operator=(const Statistics &other)
{
    receiveFrames = other.receiveFrames ;
    receivePackets = other.receivePackets ;
    sendFrames = other.sendFrames ;
    sendPackets = other.sendPackets ;
    return *this;
}

Statistics Statistics::operator+(const Statistics &other)
{
    Statistics temp;
    temp.receiveFrames = receiveFrames + other.receiveFrames ;
    temp.receivePackets = receivePackets + other.receivePackets ;
    temp.sendFrames = sendFrames + other.sendFrames ;
    temp.sendPackets = sendPackets + other.sendPackets ;
    return temp;
}

Statistics Statistics::operator+=(const Statistics &other)
{
    *this = *this + other;
    return *this;
}

QString Statistics::toString() const
{
    QString t;
    t += QObject::tr("Received frames: %1\n").arg( receiveFrames ) +
    QObject::tr("Received packets: %1\n").arg( receivePackets ) +
    QObject::tr("Sent frames: %1\n").arg( sendFrames ) +
    QObject::tr("Sent packets: %1\n").arg( sendPackets ) ;
    return t;
}
