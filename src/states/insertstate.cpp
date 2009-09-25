#include <QtGui/QGraphicsSceneMouseEvent>
#include "insertstate.h"
#include "mycanvas.h"
#include "insertrect.h"
#include "addcommand.h"

insertState::insertState(myCanvas *s) : abstractState(s)
{
    InsertRect = new insertRect;
    scene->addItem(InsertRect);
}

insertState::~insertState()
{
    scene->removeItem(InsertRect);
    delete InsertRect;
}

void insertState::hideState()
{
    InsertRect->setPos(-100,-100);
}

void insertState::mouseMove(QGraphicsSceneMouseEvent *event)
{
    InsertRect->moving( event->scenePos() );
}

void insertState::mousePress(QGraphicsSceneMouseEvent *event)
{
    if ( !InsertRect->isReadyInsert() ) return;
     // Добавляем устройство на сцену
    addCommand *command = new addCommand( scene , event->scenePos() , scene->nowType );
    scene->commandStack.push(command);
    scene->myModified = true;
}


