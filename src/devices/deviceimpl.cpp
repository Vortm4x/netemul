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
#include "deviceimpl.h"
#include "devicenotedialog.h"

DeviceImpl::DeviceImpl(QObject *parent) : QObject(parent)
{
}
//----------------------------------------------------------

void DeviceImpl::showDeviceNoteDialog()
{
    deviceNoteDialog *d = new deviceNoteDialog;
    d->setDevice( new DeviceSetting(this) );
    d->exec();
}

void DeviceImpl::write(QDataStream &stream) const
{
    stream << myNote;
}

void DeviceImpl::read(QDataStream &stream)
{
    stream >> myNote;
}



