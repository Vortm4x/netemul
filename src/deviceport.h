#ifndef DEVICEPORT_H
#define DEVICEPORT_H

#include <QQueue>
#include "frame.h"

class macAddress;
class cableDev;

/*!
  Класс представляе собой место соединения кабеля с устройством(сокет, гнездо, отверстие).
  Необходим этот класс из за того что устройство как абстрактный класс имеет гнезда,
  но обработка у всех отличается, для того что бы оставить общее и разграничить разное,
  был сделан этот класс.
*/
class devicePort
{
public:
    devicePort();
    ~devicePort();
    bool isBusy() const { return !senderQueue.isEmpty() || !receiveQueue.isEmpty(); }
    bool isConnect() const { return myConnect;}
    void setNum(int n) { myNum = n; }
    int num() const { return myNum; }
    void pushToSend(frame &f) { senderQueue.enqueue(f); }
    bool isCableConnect(const cableDev *c) const;
#ifndef __TESTING__
    void setConnect(bool cur,cableDev *cable);
    frame popFromReceive() { return receiveQueue.dequeue(); }
    void sendFrame(frame t);
    void receiveFrame(QByteArray &b);
    void queueEvent();
    void setChecked(bool c);
    bool hasReceive() const { return !receiveQueue.isEmpty(); }
#endif
private:
    int myNum;
    QQueue<frame> senderQueue;
    QQueue<frame> receiveQueue;
    cableDev *myCable;
    bool myConnect;
};
//------------------------------------------------------


#endif // DEVICEPORT_H
