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
    setToolTip(trUtf8("Соединяет порты:<br>%1 - %2").arg(myStartPort->name()).arg(myEndPort->name()));
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
    if ( isSelected() ) painter->setPen(QPen(Qt::blue,3)); // Если выделен рисуем синим
    else if ( myChecked ) painter->setPen(QPen(Qt::magenta,3));
    else painter->setPen(QPen(Qt::black,3)); // Иначе черным
    QLineF centerLine(myStartDev->getPointCable(myEndDev->pos()),
                 myEndDev->getPointCable(myStartDev->pos()));
    setLine(centerLine); // И мы её и ставим
    painter->drawLine(line()); // А потом рисуем заново
    foreach ( bitStream *i , myStreams ) {
        painter->setPen(QPen( i->color ,4));
        painter->drawPoint( line().pointAt( i->pos ) );
    }
}
//--------------------------------------------------------------------
void cableDev::updatePosition()
{
    QLineF line(myStartDev->getPointCable(myEndDev->pos()),
                 myEndDev->getPointCable(myStartDev->pos()));
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
    t->color = Qt::red;
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
/*!

*/
bool cableDev::accupant(int u, devicePort *d)
{
    if ( u == 0 ) return !isBusy();
    device *p;
    if ( d == myStartPort ) p = myEndDev;
    else p = myStartDev;
    if ( p->type() == hub ) return p->accupant(u-1);
    return true;
}
//---------------------------------------------------------

