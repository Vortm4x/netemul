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
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QMap>
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
    if ( coordMap.count() ) scene->QGraphicsScene::mouseMoveEvent(event);
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
                coordMap.insert( i , i->scenePos());
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
    if ( coordMap.count() ) {
        bool needReturn = false;
        QGraphicsItem *curDevice;
        QPointF curPoint;

        QMapIterator<QGraphicsItem*,QPointF> i(coordMap);
        while (i.hasNext()) {
            i.next();
            curDevice = i.key();
            curPoint = i.value();
            if ( curDevice->type() == textItem::Type ) continue;

            itemList underItems = curDevice->collidingItems();
            if ( !scene->sceneRect().contains( curDevice->pos()) || filterDevices(underItems).count() ) {
                needReturn = true;
                break; // while( i.has...)
            }
        }

        if ( !needReturn ) {
            QMap<QGraphicsItem*, QPointF> old = coordMap;
            scene->calibrateAll( coordMap.keys() );
            QMap<QGraphicsItem*, QPointF> rec;
            foreach ( QGraphicsItem* i ,scene->selectedItems() ) {
                if ( i->type() != cableDev::Type )
                    rec.insert(i, i->scenePos() );
             }
            moveCommand *c = new moveCommand(scene,old, rec);
            scene->commandStack.push(c);
            coordMap.clear();
            return;
        }
        scene->putItems(coordMap);
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

itemList moveState::filterDevices(itemList list)
{
    itemList temp;
    foreach ( QGraphicsItem *i , list )
        if ( scene->isDevice(i)  ) temp << i;
    return temp;
}
