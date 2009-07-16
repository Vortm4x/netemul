#include "device.h"
#include <QMenu>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>

device::device()
{
    myReady = 0;
    devRect = QRect(rectDevX,rectDevY,rectDevWidth,rectDevHeight);
    setFlag(QGraphicsItem::ItemIsMovable, true); // Устройство можно двигать
    setFlag(QGraphicsItem::ItemIsSelectable, true); // И выделять
}

device::~device()
{
    qDeleteAll(mySockets);
    mySockets.clear();
}

void device::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection(); // Снять все выделения на сцене
    setSelected(true); // Выделить текущуе устройство
    popUpMenu->exec(event->screenPos()); // Запустить контекстное меню в текущей позиции
}

void  device::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    dialog();
    scene()->clearSelection();
}


/*!
  Функция перебирает все порты устройства и если их буферы содержат кадры,
  то отправляет их.
*/
void device::sendEvent()
{
    foreach ( devicePort *i , mySockets)
         i->queueEvent();
}
//----------------------------------------------------------------
/*!
    Определяет возможна ли передача через это устройство, т.к. оно является
    разделяемой средой передачи.
    @return true - если передача возможна, false - в противном случае.
*/
bool device::accupant()
{
    foreach ( devicePort *i, mySockets )
        if ( i->isConnect() || i->isBusy()) return false;
    foreach ( devicePort *i, mySockets )
        if ( !i->accupant() ) return false;
    return true;
}
//-----------------------------------------------------------------
