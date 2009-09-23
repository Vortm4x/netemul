#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QGraphicsLineItem>
#include "connectdialog.h"
#include "cablestate.h"
#include "mycanvas.h"
#include "device.h"

cableState::cableState(myCanvas *s) : abstractState(s)
{
    line = 0; // Провода нет
}

void cableState::mouseMove(QGraphicsSceneMouseEvent *event)
{
    // Заново прорисовываем линию от начала кабеля до Текущей точки
    if ( line ) line->setLine(QLineF( line->line().p1(), event->scenePos()));
}

void cableState::mousePress(QGraphicsSceneMouseEvent *event)
{
    if ( scene->items( event->scenePos() ).isEmpty() ) return; // не будем водить кабеля из пустых мест
    line =  new QGraphicsLineItem(QLineF(event->scenePos(), event->scenePos()));
    //Создадим временную линию, выставим её свойства и добавим на сцену
    line->setPen(QPen(Qt::black,2));
    scene->addItem(line);
}

void cableState::mouseRelease(QGraphicsSceneMouseEvent*)
{
    if ( !line ) return;
    QString start,end;
    device *startItem = scene->deviceInPoint(line->line().p1()) ;
    device *endItem = scene->deviceInPoint(line->line().p2()) ;
    scene->removeItem(line);
    delete line;
    line = 0; // Линию временную делаем указателем на нуль
    if ( !startItem || !endItem ) return;
    if ( device::isConnectDevices(startItem, endItem) ) return;
    if ( startItem == endItem ) return;
    connectDialog *conDialog = new connectDialog(startItem,endItem);
    bool canCreate = conDialog->exec();
    start = conDialog->getStart();
    end = conDialog->getEnd();
    delete conDialog;
    if ( !canCreate ) return;
    // Вообщем так ... если уствойства есть под обоими концами
    // и эти устройства различны то мы создаем этот кабель! #НИХРЕНА НЕ#Проверено все ок =)
    scene->createConnection( startItem , endItem , start , end );
}
