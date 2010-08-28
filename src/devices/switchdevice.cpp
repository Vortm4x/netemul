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
#include "switchdevice.h"
#include "tableswitch.h"
#include "switchproperty.h"
#include "switchchip.h"
#include "appsetting.h"
#include "virtualnetworkdialog.h"

SwitchDevice::SwitchDevice(QObject *parent) : BoxDevice(parent)
{
    int c = appSetting::defaultSwitchCount();
    myManual = appSetting::defaultSwitchManual();
    chip = new switchChip(c);
    setNote(tr("<b>Switch</b><!--You can use HTML.-->"));
}

SwitchDevice::~SwitchDevice()
{
    delete chip;
}

void SwitchDevice::write(QDataStream &stream) const
{
    stream << switchDev;
    BoxDevice::write(stream);
}

void SwitchDevice::read(QDataStream &stream)
{
    BoxDevice::read(stream);
}

void SwitchDevice::dialog()
{
    switchProperty *d = new switchProperty;
    d->setSwitch( new boxSetting(this) );
    d->show();
}

void SwitchDevice::tableDialog()
{
    switchTableSetting *set = new switchTableSetting(this);
    tableSwitch *t = new tableSwitch(set);
    t->exec();
    delete t;
    delete set;
}

void SwitchDevice::showVirtualNetworkDialog()
{
    virtualNetworkDialog *d = new virtualNetworkDialog;
    d->setDevice( new virtualNetworkSetting(this) );
    d->show();
}

switchChip* SwitchDevice::concreteChip()
{
    return static_cast<switchChip*>(chip);
}

void SwitchDevice::secondTimerEvent()
{
    chip->secondTimerEvent();
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
switchModel* switchTableSetting::switchTable()
{
    return sw->concreteChip()->modelAt( sw->concreteChip()->vlanAt(0) );
}


