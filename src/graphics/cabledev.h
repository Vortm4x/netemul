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
#ifndef CABLEDEV_H
#define CABLEDEV_H

#include <QGraphicsObject>
#include <QPainter>
#include <QQueue>

class DevicePort;

static const qreal NORMAL_SIZE = 2.5;
static const qreal COLLISION_SIZE = 5.0;
static const int MINIMUM_DEAD = 3;
static const int TIME_BEFORE_DEAD = 17;
static const int PERCENT_DEAD = 25;

class Cable;
typedef QList<Cable*> CableList;

struct bitStream {
    QByteArray data;
    QBrush color;
    qreal pos;
    qreal size;
};

class Cable : public QGraphicsObject
{
    Q_OBJECT
public:
    enum { Type = UserType + 1 };
    enum { normal = 3 , broadcast = 4 };
    int type() const { return Type; }
    QRectF boundingRect() const {
        return QRectF(startItem->pos(),endItem->pos()).normalized(); //.adjusted(-5,-5,5,5);
    }
    bool isCollisionCable() const { return isCollision; }    
    Cable(QGraphicsObject *parent = 0);
    ~Cable();
    void updatePosition(); // Обновление прорисовки
    void motion();
    bool isBusy() const { return fromStartQueue.count() || fromEndQueue.count(); }
    bool isBusy(const DevicePort *d);    
    int isShared() const { return myShared; }
    void input(QByteArray b,DevicePort *cur);
    void setChecked(bool c) { myChecked = c; update(); }
    bool isChecked() const { return myChecked; }            

    void startCollision();

public:

    void setStartItem(QGraphicsItem *item) { startItem = item; }
    void setEndItem(QGraphicsItem *item) { endItem = item; }

    void setStartPort( DevicePort *port);
    void setEndPort( DevicePort *port );
    DevicePort* startPort() { return myStartPort; }
    DevicePort* endPort() { return myEndPort; }

    QPointF startPos() const { return startItem->pos(); }
    QPointF endPos() const { return endItem->pos(); }

private:    
    QGraphicsItem *startItem;
    QGraphicsItem *endItem;
    DevicePort *myStartPort;
    DevicePort *myEndPort;    

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                QWidget *widget = 0); // Как будем рисовать
private:
    void killRandomPackets(QQueue<bitStream*> stream);
    void killCurrentPackets();
    bool isCollision;
    bool myChecked;
    bool myShared;
    QQueue<bitStream*> fromStartQueue;
    QQueue<bitStream*> fromEndQueue;
    int mySpeed;
};

#endif // CABLEDEV_H
