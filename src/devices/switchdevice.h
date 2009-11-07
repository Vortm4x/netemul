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
#ifndef SWITCHDEVICE_H
#define SWITCHDEVICE_H

#include "boxdevice.h"
#include "macaddress.h"

class frame;
class switchModel;
class switchTableSetting;
class switchChip;

// Пока такое коротенькое объявление для свитча
// Все его комментарии действительны для других устройств
class switchDevice : public boxDevice
{
    Q_OBJECT
public:
    friend class switchTableSetting;
    enum { switchDev = 5 };
    int type() const { return switchDev; }
    switchDevice(int c = 0); //Пока конструктор и прорисовка
    ~switchDevice();
    void updateMac(int u);
    void dialog();
    void tableDialog();
    bool hasTable() const { return true; }
    bool isManagedVirtualNetwork() const { return ( myManual ) ? true : false; }
    void showVirtualNetworkDialog();
    switchChip* concreteChip();
    void secondTimerEvent();
    QString deviceName() const { return "switch"; }
    QString deviceCommandName() const { return tr("Switch"); }
    QString tableName() const { return tr("Switching table"); }
protected:
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
};

class switchTableSetting {
public:
    switchTableSetting(switchDevice *s) : sw(s) { }
    switchModel* switchTable();
    int socketsCount() const { return sw->chip->socketsCount(); }
    void removeFromTable(int row);
    devicePort* socket(const QString &s) { return sw->chip->socket(s); }
private:
    switchDevice *sw;
};

class virtualNetworkSetting {
public:
    virtualNetworkSetting( switchDevice *device ) : myDevice(device) { }
private:
    switchDevice *myDevice;
};

#endif // SWITCHDEVICE_H
