#ifndef SENDSTATE_H
#define SENDSTATE_H

#include "abstractstate.h"

class device;
class sendEllipse;

class sendState : public abstractState
{
public:
    sendState(myCanvas *s);
    ~sendState();
    enum { noSendItem = 0 , oneSendItem = 1 };
    void mouseMove(QGraphicsSceneMouseEvent *event);
    void mousePress(QGraphicsSceneMouseEvent*);
    void mouseRelease(QGraphicsSceneMouseEvent*) { }
    void goInsert() { }
    void goCable() { }
    void goText() { }
    void hideState();
private:
    // All temp transport varios
    int messageSize;
    int mySendState;
    bool broadcast;
    QString receiverIp;
    device *senderDevice;
    int protocol;
    sendEllipse *SendEllipse; // Кружочек для выделения отправителя и получателя
};

#endif // SENDSTATE_H
