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
#ifndef ROUTERDEVICE_H
#define ROUTERDEVICE_H

#include "smartdevice.h"

class routerDevice : public smartDevice
{
    Q_OBJECT
public:
    enum { routerDev = 7 };
    int type() const { return routerDev; }
    routerDevice(int c = 0);
    QString deviceName() const { return "router"; }
    QString deviceCommandName() const { return tr("Router"); }
    bool canManageInterface() const { return false; }
    void dialog();
protected:
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
public slots:
    void setSocketsCount(int n);
};

class routerSetting : public smartSetting
{
public:
    routerSetting(routerDevice *r) : smartSetting(r) , rd(r) { }
    void setSocketsCount(int n) { rd->setSocketsCount(n); }
private:
    routerDevice *rd;
};


#endif // ROUTERDEVICE_H
