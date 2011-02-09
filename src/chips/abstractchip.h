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

class DevicePort;
class IpPacket;

/*!
  Класс являеться абстрактным устройством приема и обработки кадров.
  Являеться предком для interface и boxDevice этот класс уже дает возможность вести статистику
  входящих кадров и пакетов, поддерживает обработку и передачу кадров на уровень выше.
*/
class AbstractChip : public QObject
{
    Q_OBJECT
    Q_PROPERTY( QString ip READ ipString WRITE setIp )
    Q_PROPERTY( QString mac READ macString WRITE setMac )
    Q_PROPERTY( QString mask READ maskString WRITE setMask )
    Q_PROPERTY( QVariant statistics READ statisticsObject )
public:
    AbstractChip(QObject *parent = 0);
    virtual ~AbstractChip() { }
    virtual void receiveEvent(Frame &fr,DevicePort *sender) = 0;
    virtual int trafficDigit() const = 0;
    QString statisticsString() const { return myStatistics.toString(); }    
    QVariant statisticsObject() const;
    Q_INVOKABLE void setStatisticsObject(StatisticsObject *obj);
    void checkReceive(Frame &f);
    void checkSend(Frame &f);
    MacAddress mac() const { return myMac; }
    IpAddress ip() const { return myIp; }
    IpAddress mask() const { return myMask; }
    QString ipString() const { return myIp.toString(); }
    QString macString() const { return myMac.toString(); }
    QString maskString() const { return myMask.toString(); }
    void setMac(const QString &m) { myMac.setMac(m); }
    void setMac(const MacAddress &m) { myMac = m; }
    void setIp(const QString &str) { if ( !str.isEmpty() ) myIp.setIp(str); }
    void setMask(const QString &str) { if ( !str.isEmpty() ) myMask.setIp(str); }
    void setIp(const IpAddress address) { myIp = address; }
    void setMask(const IpAddress address) { myMask = address; }
    virtual void write(QDataStream &stream) const;
    virtual void read(QDataStream &stream);
    Statistics statistics() { return myStatistics; }
    quint64 countRecFrame() { return myStatistics.receiveFrames; }
    quint64 countRecPacket() { return myStatistics.receivePackets; }
    quint64 countSendFrame() { return myStatistics.sendFrames; }
    quint64 countSendPacket() { return myStatistics.sendPackets; }
    void resetStatics();
signals:
    void sendData(Frame,QString);
    void receiveData(Frame,QString);
protected:
    Statistics myStatistics;
    IpAddress myIp;
    IpAddress myMask;
    MacAddress myMac;
};


#endif // ABSTRACTCHIP_H
