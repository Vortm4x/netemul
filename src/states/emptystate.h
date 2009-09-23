#ifndef EMPTYSTATE_H
#define EMPTYSTATE_H

#include "abstractstate.h"

class emptyState : public abstractState
{
public:
    emptyState(myCanvas *s) : abstractState(s) { }
    void mouseMove(QGraphicsSceneMouseEvent*) { }
    void mousePress(QGraphicsSceneMouseEvent*) { }
    void mouseRelease(QGraphicsSceneMouseEvent*) { }
};

#endif // EMPTYSTATE_H
