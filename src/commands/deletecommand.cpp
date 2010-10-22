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
#include "deletecommand.h"
#include "mycanvas.h"
#include "cabledev.h"
#include "device.h"
#include "textitem.h"

deleteCommand::deleteCommand(MyCanvas *s, QList<QGraphicsItem*> list)
{
    scene = s;
    foreach ( QGraphicsItem *i, list) {
        if ( scene->isDevice(i) ) {
            Device *dev = qgraphicsitem_cast<Device*>(i);
            extractDevices << dev;
            foreach ( Cable *j, dev->cables() ) extractCables << j;
        }
        else if ( i->type() == Cable::Type ) extractCables << qgraphicsitem_cast<Cable*>(i);
        else if (i->type() == TextItem::Type )  extractText << qgraphicsitem_cast<TextItem*>(i);
    }
    setText(QObject::tr("Delete"));
}

deleteCommand::~deleteCommand()
{
    if ( isOnScene ) return;
    qDeleteAll(extractDevices);
    qDeleteAll(extractCables);
    qDeleteAll(extractText);
}

void deleteCommand::undo()
{
    foreach ( Device *i, extractDevices ) scene->registerDevice(i);
    foreach ( Cable *i, extractCables ) createConnection(i);
    foreach ( TextItem *i , extractText ) scene->registerText(i);
    isOnScene = true;
}

void deleteCommand::redo()
{
    foreach ( Device *i, extractDevices ) scene->unregisterDevice(i);
    foreach ( Cable *i, extractCables ) deleteConnection(i);
    foreach ( TextItem *i, extractText ) scene->unregisterText(i);
    isOnScene = false;
}

void deleteCommand::deleteConnection(Cable *cable)
{
//    cable->unregisterCable();
    scene->unregisterCable(cable);
}

void deleteCommand::createConnection(Cable *cable)
{
//    cable->registerCable();
    scene->registerCable(cable);
}
