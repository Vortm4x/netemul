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
#include <QGraphicsLineItem>
#include "connectdialog.h"
#include "cablestate.h"
#include "mycanvas.h"
#include "device.h"

CableState::CableState(MyCanvas *s) : AbstractState(s)
{
    line = 0; // Провода нет
}

CableState::~CableState()
{
    if ( line ) {
        scene->removeItem(line);
        delete line;
    }
}

void CableState::mouseMove(QGraphicsSceneMouseEvent *event)
{
    // Заново прорисовываем линию от начала кабеля до Текущей точки
    if ( line ) line->setLine(QLineF( line->line().p1(), event->scenePos()));
}

void CableState::mousePress(QGraphicsSceneMouseEvent *event)
{
    if ( scene->items( event->scenePos() ).isEmpty() ) return; // не будем водить кабеля из пустых мест
    line =  new QGraphicsLineItem(QLineF(event->scenePos(), event->scenePos()));
    //Создадим временную линию, выставим её свойства и добавим на сцену
    line->setPen(QPen(Qt::black,2));
    scene->addItem(line);
}

void CableState::mouseRelease(QGraphicsSceneMouseEvent*)
{
    if ( !line ) return;
    QString start,end;
    Device *startItem = scene->deviceInPoint(line->line().p1()) ;
    Device *endItem = scene->deviceInPoint(line->line().p2()) ;
    scene->removeItem(line);
    delete line;
    line = 0; // Линию временную делаем указателем на нуль
    if ( !startItem || !endItem ) return;
    if ( Device::isConnectDevices(startItem, endItem) ) return;
    if ( startItem == endItem ) return;
    connectDialog *conDialog = new connectDialog(startItem,endItem);
    if ( !conDialog->exec() ) {
        delete conDialog;
        return;
    }
    start = conDialog->getStart();
    end = conDialog->getEnd();
    // Вообщем так ... если уствойства есть под обоими концами
    // и эти устройства различны то мы создаем этот кабель! #НИХРЕНА НЕ#Проверено все ок =)
    scene->createConnection( startItem , endItem , start , end );
}
