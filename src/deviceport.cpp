#include "deviceport.h"
#include "cabledev.h"
#include "frame.h"

devicePort::devicePort()
{
    myConnect = false;
    myCable = 0;
    receiveQueue.clear();
    senderQueue.clear();
}

devicePort::~devicePort()
{

}

bool devicePort::isCableConnect(const cableDev *c) const
{
    if ( myCable == c ) return true;
    return false;
}
#ifndef __TESTING__
/*!
  Достает кадр из очереди и отправляет его.
*/
void devicePort::queueEvent()
{
    if ( senderQueue.isEmpty() ) return;
    sendFrame( senderQueue.dequeue() );
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
/*!
  Отправляет кадр в сеть.
  @param t - указатель на кадр.
*/
void devicePort::sendFrame(frame t)
{
    myCable->input(t.toData(),this);
}
//-----------------------------------------------------

void devicePort::setChecked(bool c)
{
    myCable->setChecked(c);
}

void devicePort::receiveFrame(QByteArray &b)
{
    frame f(b);
    receiveQueue.enqueue(f);
}
#endif



