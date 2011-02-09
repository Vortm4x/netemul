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
#ifndef DEVICEPORT_H
#define DEVICEPORT_H

#include <QQueue>
#include "Frame.h"

class MacAddress;
class Cable;

/*!
  Класс представляе собой место соединения кабеля с устройством(сокет, гнездо, отверстие).
  Необходим этот класс из за того что устройство как абстрактный класс имеет гнезда,
  но обработка у всех отличается, для того что бы оставить общее и разграничить разное,
  был сделан этот класс.
*/
class DevicePort : public QObject
{
    Q_OBJECT
public:
    DevicePort(QObject *parent = 0);
    DevicePort(int n, QObject *parent = 0);
    ~DevicePort();
    bool isBusy() const { return !senderQueue.isEmpty() || !receiveQueue.isEmpty(); }    
    void setNum(int n) { myNum = n; }
    bool isShared() { return myShared; }
    void setShared(bool n) { myShared = n; }
    int num() const { return myNum; }
    void pushToSend(Frame &f) { senderQueue.enqueue(f); }
    bool isConnect() const { return (myCable != 0); }
    bool isCableConnect(const Cable *c) const;
    Cable* cable() const { return myCable; }
    void detectCollision() { /* MAKE IT PLEASE! */ }
    int trafficDigit() const { return senderQueue.size(); }
    void setConnect(Cable *cable);
    Frame popFromReceive() { return receiveQueue.dequeue(); }
    void receiveFrame(QByteArray &b);
    void queueEvent();
    void setChecked(bool c);
    bool hasReceive() const { return !receiveQueue.isEmpty(); }
    bool isCableBusy() const;
    void startCollision();

signals:
    void cableConnected(Cable*);

private:
    int myNum;
    QQueue<Frame> senderQueue;
    QQueue<Frame> receiveQueue;
    Cable *myCable;    
    bool myShared;
};
//------------------------------------------------------


#endif // DEVICEPORT_H
