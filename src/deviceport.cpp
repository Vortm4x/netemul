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
    return myParentDev->mac();
}

void devicePort::sendFrame(frame *temp)
{
    myParentDev->addSend(1,0);
    if ( !isConnect() ) delete temp;
    myCable->input(temp,this);
}

void devicePort::receiveFrame(frame *temp)
{
    myParentDev->addRec(1,0);
    myParentDev->receiveEvent(temp,this);
}
/**
  Достает кадр из очереди и отправляет его.
*/
void devicePort::queueEvent()
{
    if ( myQueue.isEmpty() ) return;
    frame *t = myQueue.dequeue();
    sendFrame(t);
}
//-----------------------------------------
/**
    Добавляет кадр в очередь на отправку.
    @param f - указатель на кадр подлежащий отправке.
*/
void devicePort::addToQueue(frame *f)
{
    myQueue.enqueue(f);
}
//----------------------------------------------------------------
/**
   Устанавливает статус соединения устройства. если устройство включено
   необходимо обязательно задать кабель вставленный в это устройство.
   @param cur - true если соединяем , false если отключаем.
   @param cable - указатель на подсоединяемый кабельь.
*/
void devicePort::setConnect(bool cur,cableDev *cable)
{
    myConnect = cur;
    myCable = cable;
    if ( !cur && !cable) {
        myParentDev->clearArp();
        qDeleteAll(myQueue);
        myQueue.clear();
    }
}
//----------------------------------------------------------------
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


