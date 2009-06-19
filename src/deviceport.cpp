#include "deviceport.h"
#include "abstractchip.h"
#include "boxdevice.h"
#include "frame.h"
#include <QIcon>
#include <QApplication>
#include <QStyle>
#include <QtDebug>

devicePort::devicePort()
{
    myConnect = false;
    myBusy = false;
    myCable = 0;
    myTimer = 0;
}

devicePort::~devicePort()
{
    if ( myTimer ) { killTimer(myTimer); myTimer = 0; }
    qDeleteAll(myQueue);
    myQueue.clear();
}

QIcon devicePort::connectIcon() const
{
    QIcon conIcon = qApp->style()->standardIcon( QStyle::SP_DialogApplyButton);
    QIcon badIcon = qApp->style()->standardIcon( QStyle::SP_DialogCancelButton);
    if (isConnect()) return conIcon;
    else return badIcon;
}

bool devicePort::isCompability(devicePort *one , devicePort *two)
{
    if ( one->isConnect() || two->isConnect() ) return false;
    return true;
}

macAddress devicePort::parentMac() const
{
    return parentDev()->mac();
}

void devicePort::sendFrame(frame *temp)
{
    parentDev()->addSend(1,0);
    if ( !isConnect() ) delete temp;
    myCable->input(temp,this);
}

void devicePort::receiveFrame(frame *temp)
{
    parentDev()->addRec(1,0);
    parentDev()->receiveEvent(temp,this);
}

void devicePort::timerEvent(QTimerEvent *e)
{
    Q_UNUSED(e);
    //if ( !myBusy && !myCable->model() && myCable->isBusy() ) return;
    frame *t;
    if ( !myQueue.isEmpty() ) {
        myBusy = true;
        t = myQueue.dequeue();
        sendFrame(t);
        return;
    }
    if ( myTimer ) { killTimer(myTimer); myTimer = 0 ; myBusy = false;}
}

void devicePort::addToQueue(frame *f)
{
    if ( parentDev()->mode() == abstractChip::test ) {
        sendFrame(f);
        return;
    }
    if ( !myTimer )  myTimer = startTimer(100);
    myQueue.enqueue(f);
}

QDataStream& operator<<(QDataStream &stream,const devicePort &port)
{
    stream << port.name();
    return stream;
}

QDataStream& operator>>(QDataStream &stream,devicePort &port)
{
    stream >> port.myName;
    return stream;
}


