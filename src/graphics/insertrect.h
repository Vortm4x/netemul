#ifndef INSERTRECT_H
#define INSERTRECT_H

#include <QGraphicsRectItem>

class insertRect : public QGraphicsRectItem
{
public:
    insertRect();
    void moving(QPointF p);
    bool isReadyInsert() { return collidingItems().isEmpty(); }
    void hideItem() { setPos(-100,-100); }
};

#endif // INSERTRECT_H
