#ifndef SENDELLIPSE_H
#define SENDELLIPSE_H

#include <QGraphicsEllipseItem>

class sendEllipse : public QGraphicsEllipseItem
{
public:
    enum { noSendItem = 0 , oneSendItem = 1 };
    sendEllipse();
    void chooseOneDevice();
    void moving(QPointF p);
    void hideItem() { setPos(-100,-100); }
    bool hasUnderDevice() { return collidingItems().count(); }
    QGraphicsItem* underDevice() { return collidingItems().first(); }
};

#endif // SENDELLIPSE_H
