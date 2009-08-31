#include "routerdevice.h"
#include "device.h"
#include "computer.h"
#include "hubdevice.h"
#include "switchdevice.h"
#include "cabledev.h"
#include <QMenu>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QtDebug>
#include <QPainter>

device::device(int t)
{
    devRect = QRect(device::rectDevX,device::rectDevY,device::rectDevWidth,device::rectDevHeight);
    pixmapRect = devRect.adjusted(3,3,-3,-3);
    setFlag(QGraphicsItem::ItemIsMovable, true); // Устройство можно двигать
    setFlag(QGraphicsItem::ItemIsSelectable, true); // И выделять
    switch (t) {
        case compDev : impl = new computer; break;
        case hubDev : impl = new hubDevice; break;
        case switchDev : impl = new switchDevice; break;
        case routerDev : impl = new routerDevice; break;
        default: break;
    }
    setToolTip( impl->note() );
}

device::device(QDataStream &stream)
{
    devRect = QRect(device::rectDevX,device::rectDevY,device::rectDevWidth,device::rectDevHeight);
    pixmapRect = devRect.adjusted(3,3,-3,-3);
    setFlag(QGraphicsItem::ItemIsMovable, true); // Устройство можно двигать
    setFlag(QGraphicsItem::ItemIsSelectable, true); // И выделять
    QPointF p;
    int tp;
    stream >> p;
    setPos(p);
    stream >> tp;
    switch (tp) {
        case compDev : impl = new computer; break;
        case hubDev : impl = new hubDevice; break;
        case switchDev : impl = new switchDevice; break;
        case routerDev : impl = new routerDevice; break;
        default: break;
    }
    impl->read(stream);
    setToolTip( impl->note() );
}

device::~device()
{
    delete impl;
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
        case compDev : painter->drawPixmap(pixmapRect,QPixmap(":/im/images/computer.png")); break;
        case hubDev : painter->drawPixmap(pixmapRect,QPixmap(":/im/images/hub.png")); break;
        case switchDev : painter->drawPixmap(pixmapRect,QPixmap(":/im/images/switch.png")); break;
        case routerDev : painter->drawPixmap(pixmapRect,QPixmap(":/im/images/router.png")); break;
        default: break;
    }
     // Потом картинку
    if ( isConnect() ) {
        if ( impl->isReady() ) painter->setBrush(Qt::green);
        else painter->setBrush(Qt::yellow);
    }
    else painter->setBrush(Qt::red);
    painter->drawEllipse(-17,-17,6,6);
    int traffic = impl->trafficDigit();
    if ( !traffic ) return;
    traffic = traffic/5+1;
    traffic = qMin( traffic , 40 );
    painter->setPen(QPen(Qt::green,3));
    painter->drawLine(-20, 19, -20+traffic, 19);
}

void device::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection(); // Снять все выделения на сцене
    setSelected(true); // Выделить текущуе устройство
    if ( !popUpMenu->exec(event->screenPos()) )
        scene()->clearSelection(); // Запустить контекстное меню в текущей позиции
}

void  device::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    dialog();
    scene()->clearSelection();
}
//----------------------------------------------------------------

void device::addConnection(const QString &port, cableDev *c)
{
    myCableList << c;
    impl->addConnection(port,c);
}

void device::deleteConnection(cableDev *c)
{
    myCableList.removeOne(c);
    impl->deleteConnection(c);
    update();
}

void device::updateCables()
{
    foreach ( cableDev *i , myCableList )
        i->updatePosition();
}

bool device::isConnectDevices(device *s , device *e)
{
    foreach( cableDev *i , s->myCableList )
        foreach ( cableDev *j , e->myCableList )
            if ( i == j ) return true;
    return false;
}




