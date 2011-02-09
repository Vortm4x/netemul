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
class SwitchModel;
class SwitchTableSetting;
class SwitchChip;

// Пока такое коротенькое объявление для свитча
// Все его комментарии действительны для других устройств
class SwitchDevice : public BoxDevice
{
    Q_OBJECT
public:
    enum { switchDev = 5 };
    int type() const { return switchDev; }
    SwitchDevice(QObject *parent = 0); //Пока конструктор и прорисовка
    static SwitchDevice* create(QObject *parent);
    ~SwitchDevice();
    void updateMac(int u);
    void dialog();
    void tableDialog();
    bool hasTable() const { return true; }
    bool isManagedVirtualNetwork() const { return ( myManual ) ? true : false; }
    void showVirtualNetworkDialog();
    SwitchChip* concreteChip();
    void secondTimerEvent();
    QString deviceName() const { return "switch"; }
    QString deviceCommandName() const { return tr("Switch"); }
    QString tableName() const { return tr("Switching table"); }
    QString pixmapName() const { return ":/im/images/switch.png"; }
protected:
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);

public:
    friend class SwitchTableSetting;
};

class SwitchTableSetting {
public:
    SwitchTableSetting(SwitchDevice *s) : sw(s) { }
    SwitchModel* switchTable();
    int socketsCount() const { return sw->myChip->socketsCount(); }
    void removeFromTable(int row);
    DevicePort* socket(const QString &s) { return sw->myChip->socket(s); }
private:
    SwitchDevice *sw;
};

class VirtualNetworkSetting {
public:
    VirtualNetworkSetting( SwitchDevice *device ) : myDevice(device) { }
private:
    SwitchDevice *myDevice;
};

#endif // SWITCHDEVICE_H
