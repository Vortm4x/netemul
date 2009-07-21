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
    bool isBusy() const { return myBusy; }
    void setBusy(bool cur) { myBusy = cur; }
    bool isConnect() const { return myConnect;}
    void setConnect(bool cur,cableDev *cable);
    void pushToSend(const frame &f) { senderQueue.enqueue(f); }
    frame popFromReceive() { return receiveQueue.dequeue(); }
    void sendFrame(frame t);
    void receiveFrame(QByteArray &b);
    void queueEvent();
    void setChecked(bool c);
    bool isCableConnect(const cableDev *c) const;
    bool hasReceive() const { return !receiveQueue.isEmpty(); }
private:
    QQueue<frame> senderQueue;
    QQueue<frame> receiveQueue;
    cableDev *myCable;
    bool myConnect;
    bool myBusy;
};
//------------------------------------------------------


#endif // DEVICEPORT_H
