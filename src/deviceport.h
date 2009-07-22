#ifndef DEVICEPORT_H
#define DEVICEPORT_H

#include <QQueue>
#ifndef __TESTING__
#include "frame.h"
#endif

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
    bool isBusy() const { return myBusy; }
    void setBusy(bool cur) { myBusy = cur; }
    bool isConnect() const { return myConnect;}
#ifndef __TESTING__
    void setConnect(bool cur,cableDev *cable);
    void pushToSend(const frame &f) { senderQueue.enqueue(f); }
    frame popFromReceive() { return receiveQueue.dequeue(); }
    void sendFrame(frame t);
    void receiveFrame(QByteArray &b);
    void queueEvent();
    void setChecked(bool c);
    bool isCableConnect(const cableDev *c) const;
    bool hasReceive() const { return !receiveQueue.isEmpty(); }
#endif
private:
#ifndef __TESTING__
    QQueue<frame> senderQueue;
    QQueue<frame> receiveQueue;
#endif
    cableDev *myCable;
    bool myConnect;
    bool myBusy;
};
//------------------------------------------------------


#endif // DEVICEPORT_H
