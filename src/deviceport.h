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
#include "frame.h"

class macAddress;
class cableDev;

/*!
  Класс представляе собой место соединения кабеля с устройством(сокет, гнездо, отверстие).
  Необходим этот класс из за того что устройство как абстрактный класс имеет гнезда,
  но обработка у всех отличается, для того что бы оставить общее и разграничить разное,
  был сделан этот класс.
*/
class devicePort
{
public:
    devicePort();
    devicePort(int n);
    ~devicePort();
    bool isBusy() const { return !senderQueue.isEmpty() || !receiveQueue.isEmpty(); }
    bool isConnect() const { return myConnect;}
    void setNum(int n) { myNum = n; }
    bool isShared() { return myShared; }
    void setShared(bool n) { myShared = n; }
    int num() const { return myNum; }
    void pushToSend(frame &f) { senderQueue.enqueue(f); }
    bool isCableConnect(const cableDev *c) const;
    int trafficDigit() const { return senderQueue.size(); }
    void setConnect(bool cur,cableDev *cable);
    frame popFromReceive() { return receiveQueue.dequeue(); }
    void receiveFrame(QByteArray &b);
    void queueEvent();
    void setChecked(bool c);
    bool hasReceive() const { return !receiveQueue.isEmpty(); }
    bool isCableBusy() const;
    void startCollision();
private:
    int myNum;
    QQueue<frame> senderQueue;
    QQueue<frame> receiveQueue;
    cableDev *myCable;
    bool myConnect;
    bool myShared;
};
//------------------------------------------------------


#endif // DEVICEPORT_H
