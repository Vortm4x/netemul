#include "cabledev.h"
#include "device.h"

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
    setFlag(QGraphicsItem::ItemIsSelectable, true); // Делаем наш кабель способным к выделению
}

cableDev::~cableDev()
{
    qDeleteAll(myStreams);
    myStreams.clear();
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
    foreach ( bitStream *i , myStreams ) {
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
void cableDev::input(QByteArray &b,devicePort *cur )
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
        t->direct = startToEnd;
        t->pos = 0;
    } else {
        t->direct = endToStart;
        t->pos = 1;
    }
    myStreams << t;
}
//---------------------------------------------------------
/*!
  Передает поток бит устройству получателю.
  @param t - указатель на поток.
*/
void cableDev::output(bitStream *t)
{
    if ( t->direct == startToEnd )
        endPort()->receiveFrame( t->data );
    else
        startPort()->receiveFrame( t->data );
    myStreams.removeOne(t);
    delete t;
}
//------------------------------------------------------------
/*!
  Перемещает все кадры на проводе на один шаг в нужном направлении.
*/
void cableDev::motion()
{
       qreal speed = mySpeed / line().length();
       speed += (qrand()%5)*(speed/10) - (qrand()%5)*(speed/10);
       foreach ( bitStream *i , myStreams ) {
            i->pos = i->pos + i->direct*speed;
            if ( i->pos >= 1 || i->pos <= 0) output(i);
        }
       update();
}
//-----------------------------------------------------------
QString cableDev::startSocketName() const
{
    return myStartDev->socketName(myStartPort);
}

QString cableDev::endSocketName() const
{
    return myEndDev->socketName(myEndPort);
}


