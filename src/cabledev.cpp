#include "cabledev.h"
#include "device.h"
#include "frame.h"
#include <QPainter>
#include <QtDebug>
#include <QTextItem>

cableDev::cableDev(device *start,device *end,devicePort* startInter, devicePort* endInter,int s)
{
    mySpeed = s;
    myChecked = false;
    myStartDev = start; // Выставляем наши указатели
    myEndDev = end; // из параметров конструктора
    if ( myStartDev->type() == hub || myEndDev->type() == hub ) myModel = 0;
    else myModel = 1;
    myStartPort = startInter;
    myEndPort = endInter;
    setToolTip(trUtf8("Соединяет порты:<br>%1 - %2").arg(myStartPort->name()).arg(myEndPort->name()));
    setFlag(QGraphicsItem::ItemIsSelectable, true); // Делаем наш кабель способным к выделению
}

cableDev::~cableDev()
{
    qDeleteAll(myFrames);
    myFrames.clear();
}

void cableDev::paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if ( isSelected() ) painter->setPen(QPen(Qt::blue,3)); // Если выделен рисуем синим
    else if ( myChecked ) painter->setPen(QPen(Qt::magenta,3));
    else painter->setPen(QPen(Qt::black,3)); // Иначе черным
    QLineF centerLine(myStartDev->getPointCable(myEndDev->pos()),
                 myEndDev->getPointCable(myStartDev->pos()));
    setLine(centerLine); // И мы её и ставим
    painter->drawLine(line()); // А потом рисуем заново
    foreach ( frame *temp , myFrames ) {
        painter->setPen(QPen(temp->color(),4));
        painter->drawPoint( line().pointAt( temp->pos() ) );
    }
}

void cableDev::updatePosition()
{
    QLineF line(myStartDev->getPointCable(myEndDev->pos()),
                 myEndDev->getPointCable(myStartDev->pos()));
    setLine(line);
    update(boundingRect());
}

void cableDev::input(frame *fr,devicePort *cur)
{
    if ( cur == myStartPort ) {
        fr->setDirection(frame::startToEnd);
        fr->setPos(0);
    } else {
        fr->setDirection(frame::endToStart);
        fr->setPos(1);
    }
    addFrame(fr);
}

void cableDev::output(frame *fr)
{
    if ( fr->direct() == frame::startToEnd ) {
        endPort()->receiveFrame(fr);
    } else {
        startPort()->receiveFrame(fr);
    }
    removeFrame(fr);
}

void cableDev::motion()
{
       if ( myFrames.isEmpty() ) return;
       qreal speed = mySpeed / line().length();
       speed += (qrand()%5)*(speed/10) - (qrand()%5)*(speed/10);
       foreach ( frame *temp , myFrames ) {
            if ( temp->direct() == frame::startToEnd ) {
                temp->setPos( temp->pos() + speed);
                if ( temp->pos() >= 1 ) output(temp);
            } else {
                temp->setPos( temp->pos() - speed);
                if ( temp->pos() <= 0 ) output(temp);
            }
        }
       update(boundingRect());
}

