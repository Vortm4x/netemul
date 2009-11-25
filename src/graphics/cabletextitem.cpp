#include "cabletextitem.h"
#include "cabledev.h"
#include <QPainter>


cableTextItem::cableTextItem(QGraphicsItem *parent /*= 0*/ , QGraphicsScene *scene /*= 0*/) : QGraphicsTextItem(parent,scene)
{
    setZValue(1005);
    setFont(QFont("Times", 8));
}

void cableTextItem::paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget)
{
    updatePosition();
    painter->setBrush( QBrush( QColor(255,255,255,224) ) );
    painter->drawRoundedRect(boundingRect(), 5, 5);
    QGraphicsTextItem::paint(painter,option,widget);
}

void cableTextItem::updatePosition()
{
    QPointF p;
    cableDev *cable = qgraphicsitem_cast<cableDev*>(parentItem());
    if ( myStart ) p = cable->line().pointAt(35/cable->line().length());
    else p = cable->line().pointAt(1-35/cable->line().length());
    setPos( p.x() - boundingRect().width()/2 , p.y() - boundingRect().height()/2  );
}

