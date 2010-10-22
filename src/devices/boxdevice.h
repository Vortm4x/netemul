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
#ifndef BOXDEVICE_H
#define BOXDEVICE_H

#include "device.h"
#include "boxchip.h"
#include "abstractchip.h"

class BoxDevice : public DeviceImpl
{
    Q_OBJECT
    Q_PROPERTY( bool isManual READ isManual WRITE setManual )
public:
    friend class boxSetting;
    BoxDevice(QObject *parent = 0);
    virtual ~BoxDevice();
    bool isConnectSocket(const QString &socket) const { return myChip->isConnectSocket(socket); }
    QIcon isConnectSocketIcon(const QString &socket) const;
    QStringList sockets() const { return myChip->sockets(); }
    QString socketName(const Cable *c) const { return myChip->socketName(c); }

    void setManual(bool c) { myManual = c; }
    bool isManual() { return myManual; }
    void deciSecondTimerEvent();
    int trafficDigit() const { return myChip->trafficDigit(); }
    virtual void secondTimerEvent() { }
    virtual void showLogDialog(logDialog *log) const;
    bool isBusy() const { return myChip->isBusy(); }
    bool isConnect() const { return myChip->isConnect(); }
    QList<Cable*> cableList() const { return myChip->cableList(); }
    DevicePort* findPortByName(const QString &name) const { return myChip->findPortByName(name); }

    Statistics deviceStatistics() const { return myChip->statistics(); }

    Q_INVOKABLE void setBoxChip(BoxChip *ch) {
        if ( ch ) {
            ch->setParent(this);
        }
        myChip = ch;
    }
    BoxChip* boxChip() const { return myChip; }

public slots:

    void setSocketsCount(int n) { myChip->setSocketsCount(n); }
    void setCheckedSocket(const QString &port);
protected:
    bool myManual;
    BoxChip *myChip;
    virtual void write(QDataStream &stream) const;
    virtual void read(QDataStream &stream);    
};

class boxSetting : public deviceSetting {
public:
    boxSetting(BoxDevice *h) : deviceSetting(h) , box(h) {  }
    int socketsCount() const { return box->myChip->socketsCount(); }
    QString snmpMac() const { return box->myChip->mac().toString(); }
    QString snmpIp() const { return box->myChip->ip().toString(); }
    QString snmpMask() const { return box->myChip->mask().toString(); }
    void setMac(const QString &str) { box->myChip->setMac(str); }
    void setIp(const QString &str) { box->myChip->setIp(str); }
    void setMask(const QString &str) { box->myChip->setMask(str); }
    QString statics() const { return box->myChip->statisticsString(); }
    bool isManual() const { return box->isManual(); }
    void setManual(bool b) { box->setManual(b); }
    void reset() { box->myChip->resetStatics(); }
    bool setSocketsCount(int n) { return box->myChip->setSocketsCount(n); }
private:
    BoxDevice *box;
};

#endif // BOXDEVICE_H
