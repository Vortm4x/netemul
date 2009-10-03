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
#include "hubproperty.h"
#include "hubdevice.h"
#include "appsetting.h"
#include "hubchip.h"

hubDevice::hubDevice(int c /* = 0 */ )
{
    if (!c) c = appSetting::defaultHubCount();
    myManual = appSetting::defaultHubManual();
    chip = new hubChip(c);
    setNote(tr( "<b>Hub</b><!--You can use HTML.-->" ) );
    collision = 0;
}

hubDevice::~hubDevice()
{
    delete chip;
}


void hubDevice::detectCollision()
{
    collision++;
    chip->detectCollision();
}

void hubDevice::read(QDataStream &stream)
{
    boxDevice::read(stream);
    stream >> collision;
}

void hubDevice::write(QDataStream &stream) const
{
    stream << hubDev;
    boxDevice::write(stream);
    stream << collision;
}

void hubDevice::dialog()
{
    hubProperty *d = new hubProperty;
    hubSetting *set = new hubSetting(this);
    d->setHub(set);
    d->exec();
    delete set;
}

