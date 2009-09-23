#ifndef INSERTSTATE_H
#define INSERTSTATE_H

#include "abstractstate.h"

class insertRect;

class insertState : public abstractState
{
public:
    insertState(myCanvas *s);
    ~insertState();
    void mouseMove(QGraphicsSceneMouseEvent *event);
    void mousePress(QGraphicsSceneMouseEvent *event);
    void mouseRelease(QGraphicsSceneMouseEvent*) { }
    void goText() { }
    void goSend() { }
    void hideState();
private:
    insertRect *InsertRect; // Прямоугольныник для вставки
};

#endif // INSERTSTATE_H
