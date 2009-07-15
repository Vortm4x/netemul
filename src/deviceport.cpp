#include "deviceport.h"
#include "boxdevice.h"
#include "frame.h"
#include <QIcon>
#include <QApplication>
#include <QStyle>

devicePort::devicePort()
{
    myConnect = false;
    myBusy = false;
    myCable = 0;
}

devicePort::~devicePort()
{
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
/*!
  Достает кадр из очереди и отправляет его.
*/
void devicePort::queueEvent()
{
    if ( myQueue.isEmpty() ) { myBusy = false; return; }
    // BUILDING !!!
    if ( !myBusy && !myCable->model() ) {
        if ( !(myBusy = accupant()) ) return;
    }
    // REFAKTORING LATER
    frame *t = myQueue.dequeue();
    sendFrame(t);
}
//---------------------------------------------------
/*!
   Устанавливает статус соединения устройства. если устройство включено
   необходимо обязательно задать кабель вставленный в это устройство.
   @param cur - true если соединяем , false если отключаем.
   @param cable - указатель на подсоединяемый кабель.
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
//---------------------------------------------------
/*!
  Отправляет кадр в сеть.
  @param t - указатель на кадр.
*/
void devicePort::sendFrame(frame *t )
{
    myParentDev->addSend(1,0);
    QByteArray b;
    QDataStream s( &b , QIODevice::WriteOnly );
    s << *t;
    myCable->input(b,this);
}
//-----------------------------------------------------



