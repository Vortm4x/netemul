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
#ifndef SHAREBUS_H
#define SHAREBUS_H

#include "device.h"
class QPainter;
class DevicePort;

// Этот класс представляет собой общую шину, он тоже вынесен в отдельный класс
// который унаследован от device , так как у общей шины будет отличаться
// реализация и отображение от остальных устройств
//
class ShareBus : public DeviceImpl
{
public:
    enum { defaultWidth = 200 };
    ShareBus(int c);
    DevicePort* addInterface(QString str,int t);
    void dialog() { }
    bool hasTable() const { return false; }
private:
    int widthDev; // Это длина нашего устройства общей шины
    //и она может изменяться в отличии от других устройств
};

#endif // SHAREBUS_H
