#ifndef CABLESTATE_H
#define CABLESTATE_H

#include "abstractstate.h"

class QGraphicsLineItem;

class cableState : public abstractState
{
public:
    cableState(myCanvas *s);
    ~cableState();
    void mouseMove(QGraphicsSceneMouseEvent *event);
    void mousePress(QGraphicsSceneMouseEvent *event);
    void mouseRelease(QGraphicsSceneMouseEvent*);
private:
    QGraphicsLineItem *line; // Временная линия для рисования
};

#endif // CABLESTATE_H
