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
#ifndef HUBDEVICE_H
#define HUBDEVICE_H

#include "boxdevice.h"

class frame;

class hubDevice : public boxDevice
{
    Q_OBJECT
    DECLARE_PROTOTYPE(hubDevice)
public:
    int type() const { return hubDev; }
    enum { hubDev = 4 };
    Q_INVOKABLE hubDevice(int c = 0);
    ~hubDevice();
    void dialog();
    void detectCollision();
    bool isShared() const { return true; }
    QString deviceName() const { return "hub"; }
    QString deviceCommandName() const { return tr("Hub"); }
    friend class hubSetting;
protected:
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
private:
    quint32 collision;
};

class hubSetting : public boxSetting
{
public:
    hubSetting(hubDevice *d) : boxSetting(d) , hd(d) { }
    quint32 collisions() const { return hd->collision; }
    void reset() { boxSetting::reset(); hd->collision = 0; }
private:
    hubDevice *hd;
};


#endif // HUBDEVICE_H
