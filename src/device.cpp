#include "device.h"
#include <QMenu>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>

device::device()
{
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
    //scene()->setSelectionArea( QPainterPath() );
}
