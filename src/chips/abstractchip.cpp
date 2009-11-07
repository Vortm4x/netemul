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
abstractChip::abstractChip()
{
#ifndef __TESTING__
    myMac.setRandom();
    resetStatics();
#endif
}
//--------------------------------------------------------------
/*!
  Сбрасывает всю статистику, накопленную этим устройством.
*/
void abstractChip::resetStatics()
{
    myStatistics.clear();
}
//------------------------------------------------
void abstractChip::write(QDataStream &stream) const
{
    stream << myMac;
    stream << myIp;
    stream << myMask;
    stream << myStatistics;
}

void abstractChip::writeXml(QXmlStreamWriter &stream) const
{
    stream.writeStartElement("abstractchip");
    stream.writeTextElement("mac", myMac.toString() );
    stream.writeTextElement("ip", myIp.toString() );
    stream.writeTextElement("mask", myMask.toString() );
    stream.writeEndElement();
}

void abstractChip::read(QDataStream &stream)
{
    stream >> myMac;
    stream >> myIp;
    stream >> myMask;
    stream >> myStatistics;
}

void abstractChip::readXml(QXmlStreamReader &stream)
{
    while ( !stream.atEnd() ) {
        stream.readNext();
        if ( stream.isEndElement() ) break;
        if ( stream.name() == "mac" ) myMac.setMac( stream.readElementText() );
        else if ( stream.name() == "ip" ) myIp.setIp( stream.readElementText() );
        else if ( stream.name() == "mask" ) myMask.setIp( stream.readElementText() );
    }
}

void abstractChip::checkReceive(frame &f)
{
    myStatistics.incReceiveFrames();
    if ( f.type() == frame::ip ) myStatistics.incReceivePackets();
}

void abstractChip::checkSend(frame &f)
{
    myStatistics.incSendFrames();
    if ( f.type() == frame::ip ) myStatistics.incSendPackets();
}



