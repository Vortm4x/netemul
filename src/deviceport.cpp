#include "deviceport.h"
#include "cabledev.h"
#include "frame.h"

devicePort::devicePort()
{
    myConnect = false;
    myShared = false;
    myCable = 0;
}

devicePort::devicePort(int n) : myNum(n)
{
    myConnect = false;
    myShared = false;
    myCable = 0;
}

devicePort::~devicePort()
{

}

bool devicePort::isCableConnect(const cableDev *c) const
{
    if ( myCable == c ) return true;
    return false;
}

bool devicePort::isCableBusy() const
{
    return myConnect && myCable->isBusy(this);
}

#ifndef __TESTING__
/*!
  Достает кадр из очереди и отправляет его.
*/
void devicePort::queueEvent()
{
    if ( senderQueue.isEmpty() ) return;
    if ( !myShared && myCable->isShared() && myCable->isBusy(this) ) return;
    frame t = senderQueue.dequeue();
    myCable->input(t.toData(),this);
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
    if (cable) cable->insertInPort(this);
    if ( !cur && !cable) {
        senderQueue.clear();
        receiveQueue.clear();
    }
}
//---------------------------------------------------

void devicePort::setChecked(bool c)
{
    myCable->setChecked(c);
}

void devicePort::receiveFrame(QByteArray &b)
{
    frame f(b);
    receiveQueue.enqueue(f);
}

void devicePort::startCollision()
{
    if ( myConnect && !myCable->isCollisionCable() ) myCable->startCollision();
}
#endif



