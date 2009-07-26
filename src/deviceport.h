#ifndef DEVICEPORT_H
#define DEVICEPORT_H

#include <QQueue>
#ifndef __TESTING__
#endif

class macAddress;
class cableDev;
class frame;

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
    void setNum(int n) { myNum = n; }
    int num() const { return myNum; }
    void pushToSend(frame *f) { senderQueue.enqueue(f); }
    bool isCableConnect(const cableDev *c) const;
#ifndef __TESTING__
    void setConnect(bool cur,cableDev *cable);
    frame* popFromReceive() { if (receiveQueue.isEmpty() ) return 0; return receiveQueue.dequeue(); }
    void sendFrame(frame *t);
    void receiveFrame(QByteArray &b);
    void queueEvent();
    void setChecked(bool c);
#endif
private:
    int myNum;
    QQueue<frame*> senderQueue;
    QQueue<frame*> receiveQueue;
    cableDev *myCable;
    bool myConnect;
    bool myBusy;
};
//------------------------------------------------------


#endif // DEVICEPORT_H
