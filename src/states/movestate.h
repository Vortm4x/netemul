#ifndef MOVESTATE_H
#define MOVESTATE_H

#include "abstractstate.h"
#include <QPointF>

class QGraphicsItem;
class selectRect;

typedef QList<QGraphicsItem*> itemList;

class moveState : public abstractState
{
public:
    moveState(myCanvas *s);
    ~moveState();
    void mouseMove(QGraphicsSceneMouseEvent *event);
    void mousePress(QGraphicsSceneMouseEvent *event);
    void mouseRelease(QGraphicsSceneMouseEvent *event);
private:    
    itemList filterCables(itemList list);
    selectRect *SelectRect; // Временный прямоугольник для выделения
    QPointF p2Rect; // Точка начала выделения
};

#endif // MOVESTATE_H
