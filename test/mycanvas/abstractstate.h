#ifndef ABSTRACTSTATE_H
#define ABSTRACTSTATE_H

#include <QGraphicsSceneMouseEvent>

class MyCanvas;

class AbstractState
{
public:
    AbstractState();
    static void initialize(MyCanvas*);
    virtual void mouseMove(QGraphicsSceneMouseEvent*) = 0;
    virtual void mousePress(QGraphicsSceneMouseEvent*) = 0;
    virtual void mouseRelease(QGraphicsSceneMouseEvent*) = 0;
};

#endif // ABSTRACTSTATE_H
