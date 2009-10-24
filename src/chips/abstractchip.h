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
#ifndef ABSTRACTCHIP_H
#define ABSTRACTCHIP_H

#include "statistics.h"

#include "macaddress.h"
#include "ipaddress.h"
#include "frame.h"

class devicePort;
class ipPacket;

/*!
  Класс являеться абстрактным устройством приема и обработки кадров.
  Являеться предком для interface и boxDevice этот класс уже дает возможность вести статистику
  входящих кадров и пакетов, поддерживает обработку и передачу кадров на уровень выше.
*/
class abstractChip : public QObject
{
    Q_OBJECT
public:
    abstractChip();
    virtual ~abstractChip() { }
    virtual void receiveEvent(frame &fr,devicePort *sender) = 0;
    virtual int trafficDigit() const = 0;
    QString staticsString() const { return myStatistics.toString(); }
    void checkReceive(frame &f);
    void checkSend(frame &f);
    macAddress mac() const { return myMac; }
    ipAddress ip() const { return myIp; }
    ipAddress mask() const { return myMask; }
    void setMac(const macAddress &m) { myMac = m; }
    void setIp(const QString str) { if ( !str.isEmpty() ) myIp.setIp(str); }
    void setMask(const QString str) { if ( !str.isEmpty() ) myMask.setIp(str); }
    void setIp(const ipAddress address) { myIp = address; }
    void setMask(const ipAddress address) { myMask = address; }
    virtual void write(QDataStream &stream) const;
    virtual void read(QDataStream &stream);
    statistics chipStatistics() { return myStatistics; }
    quint64 countRecFrame() { return myStatistics.receiveFrames(); }
    quint64 countRecPacket() { return myStatistics.receivePackets(); }
    quint64 countSendFrame() { return myStatistics.sendFrames(); }
    quint64 countSendPacket() { return myStatistics.sendPackets(); }
    void resetStatics();
signals:
    void sendData(frame,QString);
    void receiveData(frame,QString);
protected:
    statistics myStatistics;
    ipAddress myIp;
    ipAddress myMask;
    macAddress myMac;
};


#endif // ABSTRACTCHIP_H
