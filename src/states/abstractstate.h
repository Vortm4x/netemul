#ifndef ABSTRACTSTATE_H
#define ABSTRACTSTATE_H

class myCanvas;
class QGraphicsSceneMouseEvent;

class abstractState
{
public:
    abstractState(myCanvas *s);
    enum { move = 0 , cable = 1 , insert = 2 , send = 6 , text = 8};
    virtual ~abstractState() { }
    virtual void mouseMove(QGraphicsSceneMouseEvent*) = 0;
    virtual void mousePress(QGraphicsSceneMouseEvent*) = 0;
    virtual void mouseRelease(QGraphicsSceneMouseEvent*) = 0;
    virtual void goMove();
    virtual void goInsert();
    virtual void goCable();
    virtual void goText();
    virtual void goSend();
    virtual void goEmpty();
    virtual void hideState() { }
    static abstractState* initialize(myCanvas *s);
    void goTo(int mode);
protected:
    myCanvas *scene;
};

#endif // ABSTRACTSTATE_H
