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

class HubDevice : public BoxDevice
{
    Q_OBJECT
    Q_PROPERTY( quint32 collision READ collision WRITE setCollision )
public:
    int type() const { return hubDev; }
    enum { hubDev = 4 };
    HubDevice(QObject *parent = 0);
    static HubDevice* create(QObject *parent);
    ~HubDevice();
    void dialog();
    void detectCollision();

    void setCollision(quint32 c) { m_collision = c; }
    quint32 collision() const { return m_collision; }

    bool isShared() const { return true; }
    QString deviceName() const { return "hub"; }
    QString deviceCommandName() const { return tr("Hub"); }
    QString pixmapName() const { return ":/im/images/hub.png"; }
    friend class HubSetting;
protected:
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
private:
    quint32 m_collision;
};

class HubSetting : public BoxSetting
{
public:
    HubSetting(HubDevice *d) : BoxSetting(d) , hd(d) { }
    quint32 collisions() const { return hd->m_collision; }
    void reset() { BoxSetting::reset(); hd->m_collision = 0; }
private:
    HubDevice *hd;
};


#endif // HUBDEVICE_H
