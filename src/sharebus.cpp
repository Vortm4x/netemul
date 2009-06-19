#include "sharebus.h"
#include "cabledev.h"
#include <QPainter>

shareBus::shareBus(int c)
{
    Q_UNUSED(c);
    widthDev = defaultWidth;
}

QRectF shareBus::boundingRect() const
{
    return QRectF(-widthDev/2-5, -10 , widthDev+10 , 20);
}

void shareBus::paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget)
{
    Q_UNUSED(option); // Пока не используем
    Q_UNUSED(widget); // Эти параметры
    QList<QGraphicsItem*> collides = collidingItems();
    foreach ( QGraphicsItem* item , collides) {
        if ( item->type() == cableDev::Type || item->type() == QGraphicsLineItem::Type) collides.removeOne(item);
    }
    if (isSelected()) {
        if (!collides.isEmpty())
            painter->setPen(QPen(Qt::red,5));
        else
            painter->setPen(QPen(Qt::blue,5));
    }
    else {
        painter->setPen(QPen(Qt::black,5)); // А иначе черный
        painter->setBrush(Qt::black);
    } // А иначе черный
    // (!ВНИМАНИЕ!) далее будет необходимо создать более красивое оформление
    // так что все строки сейчас здесь написанные будут еще не один раз переписаны
    // Следи за этим
    painter->drawLine(QLineF(-widthDev/2 , 0 , widthDev/2, 0 ));
    painter->setPen(QPen(Qt::black,3));
    painter->drawLine(QLineF(-widthDev/2 , 5 , -widthDev/2, -5 ));
    painter->drawLine(QLineF( widthDev/2 , 5 , widthDev/2, -5 ));
}

 QPointF shareBus::getPointCable(QPointF otherDev)
 {
     if ( otherDev.x() < pos().x() - widthDev/2 + 10 ) {
        return QPointF( pos().x() - widthDev/2 + 10 , pos().y() );
     }
     else if ( otherDev.x() > pos().x() + widthDev/2 - 10 ) {
        return QPointF( pos().x() + widthDev/2 - 10 , pos().y() );
     }
     else {
         return QPointF( otherDev.x() , pos().y() );
     }
 }

  QPainterPath shareBus::shape() const
 {
     QPainterPath path;
     path.addEllipse( QRectF(-widthDev/2-5, -22.5 , widthDev+10 , 45) );
     return path;
 }

  devicePort* shareBus::addInterface(QString str, int t)
  {
      Q_UNUSED(str);
      Q_UNUSED(t);
      return NULL;
  }

  void shareBus::write(QDataStream &stream) const
  {
      stream << busDev << pos();
  }

  void shareBus::read(QDataStream &stream)
  {
      QPointF p;
      stream >> p;
      setPos(p);
  }
