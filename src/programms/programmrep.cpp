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
#include "programmrep.h"
#include <QtDebug>

programmRep::programmRep()
{
}

programmRep::~programmRep()
{

}

/*!
  Записывает программу в поток.
  @param stream - поток для записи.
*/
void programmRep::write(QDataStream &stream) const
{
    stream << myEnable;
}
//--------------------------------------------
/*!
  Считывает программу из потока.
  @param stream - поток для чтения.
*/
void programmRep::read(QDataStream &stream)
{
    stream >> myEnable;
}
//-------------------------------------------

void programmRep::writeXml(sceneXmlWriter &stream) const
{    
    stream.writeTextElement("enable", ( myEnable ) ? "1" : "0" );
}

void programmRep::readXml(sceneXmlReader &stream)
{
    while ( !stream.atEnd() ) {
        stream.readNext();
        if ( stream.isEndElement() ) break;
        if ( stream.name() == "enable" ) myEnable = stream.readElementText().toInt();
    }
}
