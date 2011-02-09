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
#include <QMenu>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QtDebug>
#include <QPainter>
#include "mycanvas.h"
#include "routerdevice.h"
#include "device.h"
#include "computer.h"
#include "hubdevice.h"
#include "switchdevice.h"
#include "cabledev.h"

Device::Device(QObject*) : QGraphicsObject(0) , impl(0)
{
    createImplHelper();
}

Device::Device(int t) : QGraphicsObject(0)
{
    createImpl(t);    
}

Device::Device(QDataStream &stream) : QGraphicsObject(0)
{
    QPointF p;
    int tp;
    stream >> p;
    setPos(p);
    stream >> tp;
    createImpl(tp);
    impl->read(stream);    
}

void Device::createImpl(int n)
{    
    switch (n) {
    case compDev : impl = Computer::create(this); break;
    case hubDev : impl = HubDevice::create(this); break;
    case switchDev : impl = SwitchDevice::create(this); break;
    case routerDev : impl = RouterDevice::create(this); break;
    default: break;
    }
    impl->setVisualizator(this);
    setToolTip( impl->note() );
    createImplHelper();
}

void Device::createImplHelper()
{
    devRect = QRect(Device::rectDevX,Device::rectDevY,Device::rectDevWidth,Device::rectDevHeight);
    pixmapRect = devRect.adjusted(3,3,-3,-3);
    setFlag(QGraphicsItem::ItemIsMovable, true); // Устройство можно двигать
    setFlag(QGraphicsItem::ItemIsSelectable, true); // И выделять
    setFlag( QGraphicsItem::ItemSendsGeometryChanges , true );
}

void Device::setDeviceImpl(DeviceImpl *im)
{
    impl = im;
    impl->setParent(this);
    impl->setVisualizator(this);
    setToolTip( impl->note() );
    update();
}

Device::~Device()
{    
}

void Device::paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget)
{
    Q_UNUSED(option); // Пока не используем
    Q_UNUSED(widget); // Эти параметры
    QList<QGraphicsItem*> collides = collidingItems();
    foreach ( QGraphicsItem* item , collides)
        if ( item->type() != Device::Type ) collides.removeOne(item);
    QLinearGradient tempGrad(Device::rectDevX , Device::rectDevY ,-Device::rectDevX,-Device::rectDevY);
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
    painter->drawRoundedRect(devRect,5,5);
    if ( !impl ) {
        return;
    }
    painter->drawPixmap(pixmapRect,QPixmap(impl->pixmapName()));
     // Потом картинку
    if ( isConnect() ) {
        if ( impl->isReady() ) painter->setBrush(Qt::green);
        else painter->setBrush(Qt::yellow);
    }
    else {
        painter->setBrush(Qt::red);
    }
    painter->drawEllipse(-17,-17,6,6);

    int pixmapY = 0;
    foreach ( QString name , myFeatures.keys() ) {
        QString color;
        if ( myFeatures.value(name) ) {
            color = "green";
        } else {
            color = "red";
        }
        painter->drawPixmap(0,-25+pixmapY*15,25,15,QPixmap(":/im/images/label_"+name+"_"+color+".png"));
        pixmapY++;
    }

    int traffic = impl->trafficDigit();
    if ( !traffic ) return;
    traffic = traffic/5+1;
    traffic = qMin( traffic , 40 );
    painter->setPen(QPen(Qt::green,3));
    painter->drawLine(-20, 19, -20+traffic, 19);
}

void Device::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    //if ( scene()->selectedItems().count() ) return;
    MyCanvas *canva = qobject_cast<MyCanvas*>(scene());
    canva->turnToMove();
    scene()->clearSelection(); // Снять все выделения на сцене
    setSelected(true); // Выделить текущуе устройство
    if ( !popUpMenu->exec(event->screenPos()) )
        scene()->clearSelection(); // Запустить контекстное меню в текущей позиции
}

void  Device::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    MyCanvas *canva = qobject_cast<MyCanvas*>(scene());
    canva->turnToMove();
    dialog();
    scene()->clearSelection();
}
//----------------------------------------------------------------


QVariant Device::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionHasChanged && scene()) {
        // value is the new position.
        updateCables();
        return value.toPointF();
    }
    return QGraphicsItem::itemChange(change, value);
}

void Device::updateCables()
{
    foreach ( Cable *i , cables() )
        i->updatePosition();
}

bool Device::isConnectDevices(Device *s , Device *e)
{
    foreach( Cable *i , s->cables() )
        foreach ( Cable *j , e->cables() )
            if ( i == j ) return true;
    return false;
}

void Device::onImplChange()
{
    myFeatures = impl->featuresList();    
    update();
}



