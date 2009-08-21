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
    isCollision = false;
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
    else if ( isCollision ) painter->setPen(QPen(Qt::red,1.7));
    else if ( myChecked ) painter->setPen(QPen(Qt::magenta,1.7));
    else painter->setPen(QPen(Qt::black,1.7)); // Иначе черным
    painter->drawLine(line());
    painter->setPen(QPen(Qt::black,1));

    foreach ( bitStream *i , fromEndQueue ) {
        painter->setBrush(i->color);
        painter->drawEllipse( line().pointAt( i->pos ) ,i->size ,i->size);
    }

    foreach ( bitStream *i , fromStartQueue ) {
        painter->setBrush(i->color);
        painter->drawEllipse( line().pointAt( i->pos ) ,i->size ,i->size);
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
void cableDev::motion()
{
    static int n = 0;
    if ( isCollision && ++n >= TIME_BEFORE_DEAD ) {
        killCurrentPackets();
        n = 0;
    }

       qreal speed = mySpeed / line().length();
       speed += (qrand()%5)*(speed/10) - (qrand()%5)*(speed/10);
       foreach ( bitStream *i , fromStartQueue )
           if ( (i->pos += speed) > 1.0 ) {
                bitStream *t = fromStartQueue.dequeue();
                if (t->color != Qt::blue) endPort()->receiveFrame( t->data );
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
                        fromStartQueue.head()->color = Qt::blue;
                        fromEndQueue.head()->color = Qt::blue;
                        fromStartQueue.head()->size = COLLISION_SIZE;
                        fromEndQueue.head()->size = COLLISION_SIZE;
                        startCollision();
                        if ( endPort()->isShared() ) end()->detectCollision();
                        if ( startPort()->isShared() ) start()->detectCollision();
                    }
           }
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
    if ( myStartPort == d ) return !fromEndQueue.isEmpty();
    else return !fromStartQueue.isEmpty();
}

void cableDev::killRandomPackets(QQueue<bitStream*> stream)
{
    if ( stream.size() <= MINIMUM_DEAD ) foreach ( bitStream *i , stream ) i->color = Qt::blue;
    else foreach ( bitStream *i , stream )
        if ( qrand()%100 >= 100-PERCENT_DEAD ) i->color = Qt::blue;
}

void cableDev::killCurrentPackets()
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

void cableDev::startCollision()
{
    if ( fromEndQueue.isEmpty() && fromStartQueue.isEmpty() ) return;
    killRandomPackets(fromEndQueue);
    killRandomPackets(fromStartQueue);
    isCollision = true;
}


