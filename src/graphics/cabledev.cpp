#include "cabledev.h"
#include "device.h"
#include "deviceport.h"
#include "appsetting.h"

cableDev::cableDev(device *start,device *end,QString sp, QString ep,int s)
{
    mySpeed = s; // Скорость кабеля.
    myStartPort = 0;
    myEndPort = 0;
    myChecked = false;
    myStartDev = start;
    myEndDev = end;
    myStartDev->addConnection(sp,this);
    myEndDev->addConnection(ep,this);
    if ( myStartPort->isShared() || myEndPort->isShared() ) myShared = true;
    setFlag(QGraphicsItem::ItemIsSelectable, true); // Делаем наш кабель способным к выделению
    setZValue(-1000.0); // Кидаем его на самый-самый задний план
}

cableDev::~cableDev()
{
    qDeleteAll(fromStartQueue);
    fromStartQueue.clear();
    qDeleteAll(fromEndQueue);
    fromEndQueue.clear();
}
/*!
  Отрисовывает кабель.
*/
void cableDev::paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(QPen(Qt::gray,5));
    QLineF centerLine(myStartDev->pos(), myEndDev->pos());
    setLine(centerLine); // И мы её и ставим
    painter->drawLine(line()); // А потом рисуем заново
    if ( isSelected() ) painter->setPen(QPen(Qt::blue,1.7)); // Если выделен рисуем синим
    else if ( myChecked ) painter->setPen(QPen(Qt::magenta,1.7));
    else painter->setPen(QPen(Qt::black,1.7)); // Иначе черным
    painter->drawLine(line());
    painter->setPen(QPen(Qt::black,1));


//    if ( myStreams.isEmpty() ) return ;
//    qreal min = 1;
//    qreal max = 0;
//    foreach ( bitStream *i , myStreams ) {
//        if ( i->pos > max ) max = i->pos;
//        if ( i->pos < min ) min = i->pos;
//    }
//    painter->setPen(QPen(Qt::red,2));
//    painter->drawLine( line().pointAt(min) , line().pointAt(max) );

    foreach ( bitStream *i , fromEndQueue ) {
        painter->setBrush(i->color);
        painter->drawEllipse( line().pointAt( i->pos ) ,2.5 ,2.5);
    }

    foreach ( bitStream *i , fromStartQueue ) {
        painter->setBrush(i->color);
        painter->drawEllipse( line().pointAt( i->pos ) ,2.5 ,2.5);
    }

}
//--------------------------------------------------------------------
void cableDev::updatePosition()
{
    QLineF line(myStartDev->pos(), myEndDev->pos());
    setLine(line);
    update(boundingRect());
}
/*!
  Принимает кадр от устройства отправителя и начинает его транспартировку.
  @param b - поток байт(уже в таком виде нам передается кадр).
  @param cur - устройство отправитель( для определения откуда начинаем рисовать ).
*/
void cableDev::input(QByteArray b,devicePort *cur )
{
    bitStream *t = new bitStream;
    qint8 v = b.at(0);
    switch (v) {
        case normal: t->color = Qt::red; break;
        case broadcast: t->color = Qt::yellow; break;
        default: t->color = Qt::green; break;
    }
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
void cableDev::motion()
{
       qreal speed = mySpeed / line().length();
       speed += (qrand()%5)*(speed/10) - (qrand()%5)*(speed/10);
       foreach ( bitStream *i , fromStartQueue )
           if ( (i->pos += speed) > 1.0 ) {
                bitStream *t = fromStartQueue.dequeue();
                endPort()->receiveFrame( t->data );
                delete t;
           }
       foreach ( bitStream *i , fromEndQueue )
           if ( (i->pos -= speed) <= 0.0 ) {
                bitStream *t = fromEndQueue.dequeue();
                startPort()->receiveFrame( t->data );
                delete t;
           }
       update();
}
//-----------------------------------------------------------

QString cableDev::startSocketName() const
{
    return myStartDev->socketName(this);
}

QString cableDev::endSocketName() const
{
    return myEndDev->socketName(this);
}

void cableDev::deleteConnect()
{
    myStartDev->deleteConnection(this);
    myEndDev->deleteConnection(this);
}

void cableDev::insertInPort(devicePort *p)
{
    if ( !myStartPort ) myStartPort = p;
    else if ( !myEndPort ) myEndPort = p;
    else qFatal("ERROR in cable!");
}


bool cableDev::isBusy(const devicePort *d)
{
    if ( myStartPort == d ) {
        if ( fromEndQueue.size() ) return true;
        if ( myEndPort->isShared() ) return myEndDev->isSharedBusy(this);
    }
    else {
        if ( fromStartQueue.size() ) return true;
        if ( myStartPort->isShared() ) return myStartDev->isSharedBusy(this);
    }
    return false;
}
