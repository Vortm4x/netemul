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
/*!
  Достает кадр из очереди и отправляет его.
*/
void devicePort::queueEvent()
{
    // BUILDING !!!
    if ( !myCable->model() && !myBusy ) {
        if ( !accupant(1) ) return;
    }
    // REFAKTORING LATER
    if ( myQueue.isEmpty() ) { myBusy = false; return; }
    myBusy = true;
    frame *t = myQueue.dequeue();
    sendFrame(t);
}
//-----------------------------------------
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



