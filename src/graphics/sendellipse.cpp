#include "sendellipse.h"
#include <QPen>
#include <QBrush>

sendEllipse::sendEllipse()
{
    setPen(QPen(Qt::lightGray));
    setBrush(QColor(255,128,0,128));
    setPos( 0 , -100);
    setZValue(1000);
    setRect(-10,-10,20,20);
}


void sendEllipse::chooseOneDevice()
{
    setPen(QPen(Qt::green));
    setBrush(QColor(0,128,0,128));
}

void sendEllipse::moving(QPointF p)
{
    setPos(p);
}
