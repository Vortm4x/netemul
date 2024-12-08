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
#include <QGraphicsSceneMouseEvent>
#include "insertstate.h"
#include "mycanvas.h"
#include "insertrect.h"
#include "addcommand.h"

InsertState::InsertState(MyCanvas *s) : AbstractState(s)
{
    insertRect = new InsertRect;
    s->addItem(insertRect);
}

InsertState::~InsertState()
{    
    scene->removeItem(insertRect);
    delete insertRect;

}

void InsertState::hideState()
{
    insertRect->setPos(-100,-100);
}

void InsertState::mouseMove(QGraphicsSceneMouseEvent *event)
{      
    insertRect->moving( event->scenePos() );
}

void InsertState::mousePress(QGraphicsSceneMouseEvent *event)
{
    if ( !insertRect->isReadyInsert() ) return;    
     // Добавляем устройство на сцену
    AddCommand *command = new AddCommand( scene , event->scenePos() , scene->nowType );
    scene->commandStack.push(command);
    scene->myModified = true;
}


