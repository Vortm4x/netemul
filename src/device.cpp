#include "device.h"
#include <QMenu>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>

device::device()
{
    myReady = 0;
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


devicePort* device::socket(const QString name)
{
    foreach ( devicePort *i, mySockets) {
        if ( i->name() == name )
            return i;
    }
    return NULL;
}

void  device::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    dialog();
    scene()->clearSelection();
}

void device::setCheckedSocket(const QString s)
{
    foreach ( devicePort *i , mySockets )
        if (i->isConnect()) i->setChecked( i->name() == s );
}
/*!
  Функция перебирает все порты устройства и если их буферы содержат кадры,
  то отправляет их.
*/
void device::sendEvent()
{
    foreach ( devicePort *i , mySockets)
        if ( i->isConnect() ) i->queueEvent();
}
//-----------------------------------------------------------------------------
