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

HubDevice::HubDevice(QObject *parent) : BoxDevice(parent)
{    
    myChip = 0;
    m_collision = 0;
}

HubDevice* HubDevice::create(QObject *parent)
{
    HubDevice *h = new HubDevice(parent);
    h->setBoxChip(new HubChip(h));
    h->setSocketsCount( appSetting::defaultHubCount() );
    h->setManual( appSetting::defaultHubManual() );
    h->setNote(tr( "<b>Hub</b><!--You can use HTML.-->" ) );
    return h;
}

HubDevice::~HubDevice()
{
    delete myChip;
}


void HubDevice::detectCollision()
{
    m_collision++;
    myChip->detectCollision();
}

void HubDevice::read(QDataStream &stream)
{
    BoxDevice::read(stream);
    stream >> m_collision;
}

void HubDevice::write(QDataStream &stream) const
{
    stream << hubDev;
    BoxDevice::write(stream);
    stream << m_collision;
}

void HubDevice::dialog()
{
    hubProperty *d = new hubProperty;
    hubSetting *set = new hubSetting(this);
    d->setHub(set);
    d->exec();
    delete set;
}

