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
#include "device.h"
#include "addcommand.h"
#include "mycanvas.h"

addCommand::addCommand(myCanvas *s, QPointF point , int type)
{
    scene = s;
    myDevice = 0;
    myPoint = point;
    myType = type;
}

addCommand::~addCommand()
{
    if ( !isOnScene ) delete myDevice;
}

void addCommand::undo()
{
    scene->unregisterDevice(myDevice);
    isOnScene = false;
}

void addCommand::redo()
{
    if ( myDevice ) scene->registerDevice(myDevice);
    else {
        myDevice = scene->addDeviceOnScene( myPoint, myType );
        setText( QObject::tr("Add %1").arg( myDevice->deviceCommandName() ) );
    }
    isOnScene = true;
}
