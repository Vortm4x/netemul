#include <QtGui/QGraphicsSceneMouseEvent>
#include "insertstate.h"
#include "mycanvas.h"
#include "insertrect.h"

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
    scene->addDeviceOnScene(event->scenePos(), scene->nowType); // Добавляем устройство на сцену
    scene->myModified = true;
}


