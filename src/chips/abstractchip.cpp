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
#include "abstractchip.h"

/*!
  Задает устройству случайный Mac-адрес, пустой ip и маску подсети.
*/
AbstractChip::AbstractChip(QObject *parent) : QObject(parent)
{
    myMac.setRandom();
    resetStatics();
}
//--------------------------------------------------------------
/*!
  Сбрасывает всю статистику, накопленную этим устройством.
*/
void AbstractChip::resetStatics()
{
    myStatistics.clear();
}
//------------------------------------------------
void AbstractChip::write(QDataStream &stream) const
{
    stream << myMac;
    stream << myIp;
    stream << myMask;
    stream << myStatistics;
}

void AbstractChip::read(QDataStream &stream)
{
    stream >> myMac;
    stream >> myIp;
    stream >> myMask;
    stream >> myStatistics;
}

QVariant AbstractChip::statisticsObject() const
{
    QObject *o = new StatisticsObject(myStatistics);
    return qVariantFromValue( o );
}

void AbstractChip::setStatisticsObject(StatisticsObject *obj)
{
    myStatistics = obj->statistics();
    obj->deleteLater();
}

void AbstractChip::checkReceive(frame &f)
{
    myStatistics.receiveFrames++;
    if ( f.type() == frame::ip ) myStatistics.receivePackets++;
}

void AbstractChip::checkSend(frame &f)
{
    myStatistics.sendFrames++;
    if ( f.type() == frame::ip ) myStatistics.sendPackets++;
}



