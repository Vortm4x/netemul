#include <QtGui/QGraphicsSceneMouseEvent>
#include "textstate.h"
#include "mycanvas.h"

void textState::mousePress(QGraphicsSceneMouseEvent *event)
{
    scene->createTextItem(event->scenePos());
    scene->myModified = true;
    emit scene->uncheck();
    goMove();
}
