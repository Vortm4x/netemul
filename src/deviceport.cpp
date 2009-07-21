#include "deviceport.h"
#include "cabledev.h"

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
    cable->insertInPort(this);
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

bool devicePort::isCableConnect(const cableDev *c) const
{
    if ( myCable == c ) return true;
    return false;
}

void devicePort::setChecked(bool c)
{
    myCable->setChecked(c);
}

void devicePort::receiveFrame(QByteArray &b)
{
    QDataStream s(b);
    frame f;
    s >> f;
    receiveQueue.enqueue(f);
}



