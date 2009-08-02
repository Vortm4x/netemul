#include "insertrect.h"
#include <QPen>
#include <QBrush>

insertRect::insertRect()
{
    setPos( 0, -50 );
    setPen(QPen(Qt::red));
    setBrush(QColor( 128 , 0 , 0 , 64));
    setZValue(1000);
    setRect( -20 , -20 , 40 ,40);
}

void insertRect::moving(QPointF p)
{
    if ( collidingItems().isEmpty() ) {
        setPen(QPen(Qt::blue));
        setBrush( QColor( 0 , 0 , 128 , 64) );
    }
    else {
        setPen(QPen(Qt::red));
        setBrush( QColor( 128 , 0 , 0 , 64));
    }
    setPos(p);
}

