#include <QtGui/QGraphicsSceneMouseEvent>
#include "movestate.h"
#include "mycanvas.h"
#include "device.h"
#include "selectrect.h"
#include "cabledev.h"
#include "movecommand.h"

moveState::moveState(myCanvas *s) : abstractState(s)
{
    SelectRect = 0; // Выделения нет
    p2Rect = QPoint();
}

moveState::~moveState()
{
    if ( SelectRect ) {
        scene->removeItem(SelectRect);
        delete SelectRect;
    }
}

void moveState::mouseMove(QGraphicsSceneMouseEvent *event)
{
    if ( scene->coordMap.count() ) scene->QGraphicsScene::mouseMoveEvent(event);
    else  if (SelectRect) // Если есть выделение обновляем его.
       SelectRect->setRect(QRectF( event->scenePos() , p2Rect ).normalized());
}

void moveState::mousePress(QGraphicsSceneMouseEvent *event)
{
    scene->QGraphicsScene::mousePressEvent(event);
    // Если есть выделенные элементы и мы щелкаем на одном из них
    if ( (scene->selectedItems().toSet() & scene->items( event->scenePos()).toSet()).size() ) {
    // То нужно сохранить все их координаты на случай если начнется перемещение.
        foreach ( QGraphicsItem* i ,scene->selectedItems() ) {
            if ( i->type() != cableDev::Type )
                scene->coordMap.insert( i , i->scenePos());
        }
    } // Иначе создаем прямоугольник выделения.
    else {
        if ( scene->items( event->scenePos() ).count() ) return;
        SelectRect = new selectRect;
        p2Rect = QPointF( event->scenePos() );
        scene->addItem(SelectRect);
    }
}

void moveState::mouseRelease(QGraphicsSceneMouseEvent *event)
{
    scene->QGraphicsScene::mouseReleaseEvent(event);
    if ( scene->coordMap.count() ) {
        bool needReturn = false;
        QGraphicsItem *curDevice;
        QPointF curPoint;

        QMapIterator<QGraphicsItem*,QPointF> i(scene->coordMap);
        while (i.hasNext()) {
            i.next();
            curDevice = i.key();
            curPoint = i.value();

            itemList underItems = curDevice->collidingItems();
            if ( !scene->sceneRect().contains( curDevice->pos()) || filterCables(underItems).count() ) {
                needReturn = true;
                break; // while( i.has...)
            }
        }

        if ( !needReturn ) {
            QMap<QGraphicsItem*, QPointF> old = scene->coordMap;
            scene->calibrateAll( scene->coordMap.keys() );
            QMap<QGraphicsItem*, QPointF> rec;
            foreach ( QGraphicsItem* i ,scene->selectedItems() ) {
                if ( i->type() != cableDev::Type )
                    rec.insert(i, i->scenePos() );
             }
            moveCommand *c = new moveCommand(scene,old, rec);
            scene->commandStack.push(c);
            scene->coordMap.clear();
            return;
        }
        scene->putItems(scene->coordMap);
    }
    else {
        if ( !SelectRect ) return;
        QPainterPath path;
        path.addRect(SelectRect->rect());
        scene->setSelectionArea(path);
        scene->removeItem(SelectRect);
        delete SelectRect;
        p2Rect = QPoint();
        SelectRect = 0 ;
    }
}

itemList moveState::filterCables(itemList list)
{
    itemList temp = list;
    foreach ( QGraphicsItem *i , temp )
        if ( i->type() == cableDev::Type ) temp.removeOne(i);
    return temp;
}
