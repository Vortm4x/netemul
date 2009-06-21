#ifndef DEVICEPORT_H
#define DEVICEPORT_H

#include <QObject>
#include <QQueue>

#include "cabledev.h"

class macAddress;
class abstractChip;
class QIcon;
class frame;


class devicePort : public QObject
{
public:
    static bool isCompability(devicePort* one , devicePort *two);
    devicePort();
    ~devicePort();
    bool isBusy() const { return myBusy; }
    void setBusy(bool cur) { myBusy = cur; }
    bool isConnect() const { return myConnect;}
    void setConnect(bool cur,cableDev *cable) { myConnect = cur;myCable = cable; }
    QIcon connectIcon() const;
    void setName(QString temp) { myName = temp; }
    QString name() const  { return myName; }
    void setParentDev(abstractChip *temp) { myParentDev = temp; }
    abstractChip* parentDev() const { return myParentDev; }
    macAddress parentMac() const;
    void addToQueue(frame *f);
    void sendFrame(frame *temp);
    void receiveFrame(frame *temp);
    void setChecked(bool c) { myCable->setChecked(c); }
private:
    QQueue<frame*> myQueue;
    cableDev *myCable;
    bool myConnect;
    bool myBusy;
    int myTimer;
    QString myName;
    abstractChip *myParentDev;
protected:
    void timerEvent(QTimerEvent *e);
    friend QDataStream& operator<<(QDataStream &stream,const devicePort &port);
    friend QDataStream& operator>>(QDataStream &stream,devicePort &port);
};

#endif // DEVICEPORT_H
