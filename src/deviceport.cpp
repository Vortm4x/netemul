/****************************************************************************************
** NetEmul - program for simulating computer networks.
** Copyright © 2009 Semenov Pavel and Omilaeva Anastasia
**
** NetEmul is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** NetEmul is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with the NetEmul; if not, write to the Free
** Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
** 02111-1307 USA.
****************************************************************************************/
#include "deviceport.h"
#include "cabledev.h"
#include "frame.h"

DevicePort::DevicePort(QObject *parent)
{
    myShared = false;
    myCable = 0;
}

DevicePort::DevicePort(int n, QObject *parent) : QObject(parent) , myNum(n)
{    
    myShared = false;
    myCable = 0;
}

DevicePort::~DevicePort()
{

}

bool DevicePort::isCableConnect(const Cable *c ) const
{
    return ( myCable == c );
}

bool DevicePort::isCableBusy() const
{
    return myCable && myCable->isBusy(this);
}

/*!
  Достает кадр из очереди и отправляет его.
*/
void DevicePort::queueEvent()
{
    if ( senderQueue.isEmpty() ) return;
    if ( !myShared && myCable->isShared() && myCable->isBusy(this) ) return;
    Frame t = senderQueue.dequeue();
    if ( t.receiver().isBroadcast() ) t.setDifferent( Frame::BROADCAST );
    else t.setDifferent( Frame::NORMAL );
    myCable->input(t.toData(),this);
}
//---------------------------------------------------
/*!
   Устанавливает статус соединения устройства. если устройство включено
   необходимо обязательно задать кабель вставленный в это устройство.
   @param cur - true если соединяем , false если отключаем.
   @param cable - указатель на подсоединяемый кабель.
*/
void DevicePort::setConnect(Cable *cable)
{    
    myCable = cable;        
    if ( !cable) {
        senderQueue.clear();
        receiveQueue.clear();
    }
    emit cableConnected(cable);
}
//---------------------------------------------------

void DevicePort::setChecked(bool c)
{
    if ( myCable ) myCable->setChecked(c);
}

void DevicePort::receiveFrame(QByteArray &b)
{
    Frame f(b);
    receiveQueue.enqueue(f);
}

void DevicePort::startCollision()
{
    if ( myCable && !myCable->isCollisionCable() ) myCable->startCollision();
}



