#ifndef TEXTSTATE_H
#define TEXTSTATE_H

#include "abstractstate.h"

class textState : public abstractState
{
public:
    textState(myCanvas *s) : abstractState(s) { }
    void mouseMove(QGraphicsSceneMouseEvent*) { }
    void mousePress(QGraphicsSceneMouseEvent *event);
    void mouseRelease(QGraphicsSceneMouseEvent*) { }
    void goInsert() { }
    void goCable() { }
    void goSend() { }
};

#endif // TEXTSTATE_H
