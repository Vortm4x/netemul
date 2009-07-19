#include "deviceport.h"

devicePort::devicePort()
{
    myConnect = false;
    myBusy = false;
    myCable = 0;
}

devicePort::~devicePort()
{
    senderQueue.clear();
    receiveQueue.clear();
}
/*!
  Достает кадр из очереди и отправляет его.
*/
void devicePort::queueEvent()
{
    if ( senderQueue.isEmpty() ) { myBusy = false; return; }
    myBusy = true;
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
    if ( !cur && !cable)
        senderQueue.clear();
}
//---------------------------------------------------
/*!
  Отправляет кадр в сеть.
  @param t - указатель на кадр.
*/
void devicePort::sendFrame(frame t)
{
    QByteArray b;
    QDataStream s( &b , QIODevice::WriteOnly );
    s << t;
    myCable->input(b,this);
}
//-----------------------------------------------------



