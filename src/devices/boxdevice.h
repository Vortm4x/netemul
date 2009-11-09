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

class boxDevice : public deviceImpl
{
public:
    friend class boxSetting;
    boxDevice();
    virtual ~boxDevice();
    bool isConnectSocket(const QString &socket) const { return chip->isConnectSocket(socket); }
    QIcon isConnectSocketIcon(const QString &socket) const;
    QStringList sockets() const { return chip->sockets(); }
    QString socketName(const cableDev *c) const { return chip->socketName(c); }
    void addConnection(const QString &port , cableDev *c) { chip->addConnection(port,c); }
    void deleteConnection(cableDev *c) { chip->deleteConnection(c); }
    void setManual(bool c) { myManual = c; }
    bool isManual() { return myManual; }
    void deciSecondTimerEvent();
    int trafficDigit() const { return chip->trafficDigit(); }
    virtual void secondTimerEvent() { }
    virtual void showLogDialog(logDialog *log) const;
    bool isBusy() const { return chip->isBusy(); }
    statistics deviceStatistics() const { return chip->chipStatistics(); }
public slots:
    void setSocketsCount(int n) { chip->setSocketsCount(n); }
    void setCheckedSocket(const QString &port);
protected:
    bool myManual;
    boxChip *chip;
    virtual void writeXml(sceneXmlWriter &stream) const;
    virtual void write(QDataStream &stream) const;
    virtual void read(QDataStream &stream);
    virtual void readXml(sceneXmlReader &stream);
};

class boxSetting : public deviceSetting {
public:
    boxSetting(boxDevice *h) : deviceSetting(h) , box(h) {  }
    int socketsCount() const { return box->chip->socketsCount(); }
    QString snmpMac() const { return box->chip->mac().toString(); }
    QString snmpIp() const { return box->chip->ip().toString(); }
    QString snmpMask() const { return box->chip->mask().toString(); }
    void setMac(const QString &str) { box->chip->setMac(str); }
    void setIp(const QString &str) { box->chip->setIp(str); }
    void setMask(const QString &str) { box->chip->setMask(str); }
    QString statics() const { return box->chip->staticsString(); }
    bool isManual() const { return box->isManual(); }
    void setManual(bool b) { box->setManual(b); }
    void reset() { box->chip->resetStatics(); }
    bool setSocketsCount(int n) { return box->chip->setSocketsCount(n); }
private:
    boxDevice *box;
};

#endif // BOXDEVICE_H
