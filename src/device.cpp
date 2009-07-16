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

void device::paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget)
{
    Q_UNUSED(option); // Пока не используем
    Q_UNUSED(widget); // Эти параметры
    QList<QGraphicsItem*> collides = collidingItems();
    foreach ( QGraphicsItem* item , collides)
        if ( item->type() == cableDev::Type || item->type() == QGraphicsLineItem::Type) collides.removeOne(item);
    QLinearGradient tempGrad(device::rectDevX , device::rectDevY ,-device::rectDevX,-device::rectDevY);
    tempGrad.setColorAt(0,Qt::white);
    if (isSelected()) {
        if (!collides.isEmpty())
            tempGrad.setColorAt(1,Qt::red);
        else
            tempGrad.setColorAt(1,Qt::blue);
        painter->setPen(Qt::darkBlue);
    }
    else {
        painter->setPen(Qt::black); // А иначе черный
        tempGrad.setColorAt(1,Qt::white);
    }
    painter->setBrush(QBrush(tempGrad));
    painter->drawRoundedRect(devRect,5,5); //Рисуем край нашего компьютера
    switch ( impl->type() ) {
        case compDev : painter->drawPixmap(devRect,QPixmap(":/im/images/laptop.png")); break;
        case hubDev : painter->drawPixmap(devRect,QPixmap(":/im/images/modem.png")); break;
        case switchDev : painter->drawPixmap(devRect,QPixmap(":/im/images/switch.png")); break;
        case routerDev : painter->drawPixmap(devRect,QPixmap(":/im/images/router.png")); break;
        default: break;
    }
     // Потом картинку
    if ( isConnect() ) {
        if ( myReady == myCableList.count() ) painter->setBrush(Qt::green);
        else painter->setBrush(Qt::yellow);
    }
    else painter->setBrush(Qt::red);
    painter->drawEllipse(-17,-17,6,6);
}

device::~device()
{
    delete impl;
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

