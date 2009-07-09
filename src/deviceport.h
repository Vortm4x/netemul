#ifndef DEVICEPORT_H
#define DEVICEPORT_H

#include <QObject>
#include <QQueue>

#include "cabledev.h"
#include "abstractchip.h"

class macAddress;
class QIcon;
class frame;

/*!
  Класс представляе собой место соединения кабеля с устройством(сокет, гнездо, отверстие).
  Необходим этот класс из за того что устройство как абстрактный класс имеет гнезда,
  но обработка у всех отличается, для того что бы оставить общее и разграничить разное,
  был сделан этот класс.
*/
class devicePort : public QObject
{
public:
    static bool isCompability(devicePort* one , devicePort *two);
    devicePort();
    ~devicePort();
    bool isBusy() const { return myBusy; }
    void setBusy(bool cur) { myBusy = cur; }
    bool isConnect() const { return myConnect;}
    void setConnect(bool cur,cableDev *cable);
    QIcon connectIcon() const;
    void setName(QString temp) { myName = temp; }
    QString name() const  { return myName; }
    void setParentDev(abstractChip *temp) { myParentDev = temp; }
    abstractChip* parentDev() const { return myParentDev; }
    macAddress parentMac() const { return myParentDev->mac(); }
    void addToQueue(frame *f) { myQueue.enqueue(f); }
    void sendFrame(frame *t);
    void receiveFrame(QByteArray &b);
    void queueEvent();
    void setChecked(bool c) { myCable->setChecked(c); }
    bool accupant(int u) { return myCable->accupant( u, this ); }
private:
    QQueue<frame*> myQueue;
    cableDev *myCable;
    bool myConnect;
    bool myBusy;
   // int myTimer;
    QString myName;
    abstractChip *myParentDev;
protected:
    friend QDataStream& operator<<(QDataStream &stream,const devicePort &port);
    friend QDataStream& operator>>(QDataStream &stream,devicePort &port);
};
//------------------------------------------------------
inline void devicePort::receiveFrame(QByteArray &b)
{
    QDataStream s(b);
    frame *f = new frame;
    s >> *f;
    myParentDev->addRec(1,0);
    myParentDev->receiveEvent(f,this);
}
inline QDataStream& operator<<(QDataStream &stream,const devicePort &port)
{
    stream << port.name();
    return stream;
}
inline QDataStream& operator>>(QDataStream &stream,devicePort &port)
{
    stream >> port.myName;
    return stream;
}
#endif // DEVICEPORT_H
