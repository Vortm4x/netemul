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
#include "cabledev.h"
#include "deviceport.h"

Cable::Cable(QGraphicsObject *parent) : QGraphicsObject(parent) , myStartPort(0) ,
                                            myEndPort(0) ,
                                        isCollision(false), myChecked(false) ,
                                                        myShared(false) , mySpeed(5)
{       
    setFlag(QGraphicsItem::ItemIsSelectable, true); // Делаем наш кабель способным к выделению
    setZValue(-1000.0); // Кидаем его на самый-самый задний план
}

Cable::~Cable()
{
    qDeleteAll(fromStartQueue);
    fromStartQueue.clear();
    qDeleteAll(fromEndQueue);
    fromEndQueue.clear();
}
/*!
  Отрисовывает кабель.
*/
void Cable::paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(QPen(Qt::gray,5));
    QLineF center( startItem->pos() , endItem->pos() );
    painter->drawLine(center); // А потом рисуем заново
    if ( isSelected() ) painter->setPen(QPen(Qt::blue,1.7)); // Если выделен рисуем синим
    else if ( isCollision ) painter->setPen(QPen(Qt::red,1.7));
    else if ( myChecked ) painter->setPen(QPen(Qt::magenta,1.7));
    else painter->setPen(QPen(Qt::black,1.7)); // Иначе черным
    painter->drawLine(center);
    painter->setPen(QPen(Qt::black,1));

    foreach ( bitStream *i , fromEndQueue ) {
        painter->setBrush(i->color);
        painter->drawEllipse( center.pointAt( i->pos ) ,i->size ,i->size);
    }

    foreach ( bitStream *i , fromStartQueue ) {
        painter->setBrush(i->color);
        painter->drawEllipse( center.pointAt( i->pos ) ,i->size ,i->size);
    }
}
//--------------------------------------------------------------------


void Cable::updatePosition()
{
    update(boundingRect());
}

/*!
  Принимает кадр от устройства отправителя и начинает его транспартировку.
  @param b - поток байт(уже в таком виде нам передается кадр).
  @param cur - устройство отправитель( для определения откуда начинаем рисовать ).
*/
void Cable::input(QByteArray b,DevicePort *cur )
{
    bitStream *t = new bitStream;
    qint8 v = b.at(0);
    switch (v) {
        case normal: t->color = Qt::red; break;
        case broadcast: t->color = Qt::yellow; break;
        default: t->color = Qt::green; break;
    }
    t->size = NORMAL_SIZE;
    t->data = b;
    if ( cur == myStartPort ) {
        fromStartQueue.enqueue(t);
        t->pos = 0.0;
    } else {
        fromEndQueue.enqueue(t);
        t->pos = 1.0;
    }
}
//---------------------------------------------------------
/*!
 * Перемещает все кадры на проводе на один шаг в нужном направлении.
*/
void Cable::motion()
{
    static int n = 0;
    if ( isCollision && ++n >= TIME_BEFORE_DEAD ) {
        killCurrentPackets();
        n = 0;
    }

       QLineF center( startItem->pos() , endItem->pos() );
       qreal speed = mySpeed / center.length();
       speed += (qrand()%5)*(speed/10) - (qrand()%5)*(speed/10);
       foreach ( bitStream *i , fromStartQueue )
           if ( (i->pos += speed) > 1.0 ) {
                bitStream *t = fromStartQueue.dequeue();
                if (t->color != Qt::blue) {
                    endPort()->receiveFrame( t->data );
                }
                delete t;
           }
       foreach ( bitStream *i , fromEndQueue )
           if ( (i->pos -= speed) <= 0.0 ) {
                bitStream *t = fromEndQueue.dequeue();
                if ( t->color != Qt::blue ) startPort()->receiveFrame( t->data );
                delete t;
           }
       if ( myShared ) {
           if ( isCollision && fromEndQueue.isEmpty() && fromStartQueue.isEmpty() ) isCollision = false;
           if ( !isCollision ) {
               if ( !fromStartQueue.isEmpty() && !fromEndQueue.isEmpty() &&
                    fromStartQueue.head()->pos > fromEndQueue.head()->pos ) {
                        fromStartQueue.head()->color = fromEndQueue.head()->color = Qt::blue;
                        fromStartQueue.head()->size = fromEndQueue.head()->size = COLLISION_SIZE;
                        startCollision();
                        if ( endPort()->isShared() ) myEndPort->detectCollision();
                        if ( startPort()->isShared() ) myStartPort->detectCollision();
                    }
           }
       }
       update();
}
//-----------------------------------------------------------

void Cable::setStartPort(DevicePort *port)
{
    myStartPort = port;
    if ( port ) {
        myStartPort->setConnect(this);
    }
}

void Cable::setEndPort(DevicePort *port)
{
    myEndPort = port;
    if ( port ) {
        myEndPort->setConnect(this);
    }
}

bool Cable::isBusy(const DevicePort *d)
{
    if ( myStartPort == d ) return !fromEndQueue.isEmpty();
    else return !fromStartQueue.isEmpty();
}

void Cable::killRandomPackets(QQueue<bitStream*> stream)
{
    if ( stream.size() <= MINIMUM_DEAD ) foreach ( bitStream *i , stream ) i->color = Qt::blue;
    else foreach ( bitStream *i , stream )
        if ( qrand()%100 >= 100-PERCENT_DEAD ) i->color = Qt::blue;
}

void Cable::killCurrentPackets()
{
    foreach ( bitStream *i , fromEndQueue )
        if ( i->color == Qt::blue ) {
            fromEndQueue.removeOne(i);
            delete i;
        }
    foreach ( bitStream *i , fromStartQueue )
        if ( i->color == Qt::blue ) {
            fromStartQueue.removeOne(i);
            delete i;
        }
}

void Cable::startCollision()
{
    if ( fromEndQueue.isEmpty() && fromStartQueue.isEmpty() ) return;
    killRandomPackets(fromEndQueue);
    killRandomPackets(fromStartQueue);
    isCollision = true;
}


