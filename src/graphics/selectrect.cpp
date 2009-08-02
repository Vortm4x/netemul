#include "selectrect.h"
#include <QPen>
#include <QBrush>

selectRect::selectRect()
{
    setPen(QPen(Qt::blue));
    setBrush(QColor( 0 , 0 ,128 , 64));
    setZValue(1000);
    setPos( 0,0 );
}
