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

device::device(sceneXmlReader &stream)
{
    devRect = QRect(device::rectDevX,device::rectDevY,device::rectDevWidth,device::rectDevHeight);
    pixmapRect = devRect.adjusted(3,3,-3,-3);
    setFlag(QGraphicsItem::ItemIsMovable, true); // Устройство можно двигать
    setFlag(QGraphicsItem::ItemIsSelectable, true); // И выделять
    QPointF p;
    int tp = stream.attributes().value("type").toString().toInt();
    switch (tp) {
        case compDev : impl = new computer; break;
        case hubDev : impl = new hubDevice; break;
        case switchDev : impl = new switchDevice; break;
        case routerDev : impl = new routerDevice; break;
        default: break;
    }
    while ( !stream.atEnd() ) {
        stream.readNext();
        if ( stream.isEndElement() ) break;
        if ( stream.name() == "x" ) p.setX( stream.readElementText().toDouble() );
        else if ( stream.name() == "y" ) p.setY( stream.readElementText().toDouble() );
        else if ( stream.name() ==  "impl" ) impl->readXml(stream);
    }
    setPos(p);
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
        if ( item->type() == cableDev::Type || item->type() == QGraphicsLineItem::Type
             || item->type() == textItem::Type ) collides.removeOne(item);
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
    //if ( scene()->selectedItems().count() ) return;
    myCanvas *canva = qobject_cast<myCanvas*>(scene());
    canva->turnToMove();
    scene()->clearSelection(); // Снять все выделения на сцене
    setSelected(true); // Выделить текущуе устройство
    if ( !popUpMenu->exec(event->screenPos()) )
        scene()->clearSelection(); // Запустить контекстное меню в текущей позиции
}

void  device::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    myCanvas *canva = qobject_cast<myCanvas*>(scene());
    canva->turnToMove();
    dialog();
    scene()->clearSelection();
}
//----------------------------------------------------------------

void device::writeXml(sceneXmlWriter &stream) const
{
    stream.writeStartElement("device");
    stream.writeAttribute("type" , QString::number(impl->type()) );
    stream.writeStartElement("x");
    stream.writeCharacters( QString::number( pos().x() ) );
    stream.writeEndElement();
    stream.writeStartElement("y");
    stream.writeCharacters( QString::number( pos().y() ) );
    stream.writeEndElement();

    stream.writeStartElement("impl");
    impl->writeXml(stream);
    stream.writeEndElement();

    stream.writeEndElement();
}

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




