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
#include "routerdevice.h"
#include "routerproperty.h"
#include "routemodel.h"
#include "appsetting.h"
#include <QMessageBox>

routerDevice::routerDevice(int c /* = 0 */)
{
    if ( !c ) c = appSetting::defaultRouterCount();
    for ( int i = 0 ; i <  c ; i++ )
        addInterface(tr("LAN%1").arg(i+1));
    setNote(tr("<b>Router</b><!--You can use HTML.-->"));
}

void routerDevice::dialog()
{
    routerProperty *d = new routerProperty;
    routerSetting *set = new routerSetting(this);
    d->setRouter(set);
    d->exec();
    delete d;
    delete set;
}

void routerDevice::write(QDataStream &stream) const
{
    stream << routerDev;
    smartDevice::write(stream);
}

void routerDevice::read(QDataStream &stream)
{
   smartDevice::read(stream);
}

void routerDevice::setSocketsCount(int n)
{
    foreach ( interface *i , myInterfaces )
        if ( i->isConnect() ) {
            QMessageBox::warning(0,tr("Error"), tr("To change the number of ports, disconnect all cables!"),
                                 QMessageBox::Ok , QMessageBox::Ok);
            return;
        }
    qDeleteAll(myInterfaces);
    myInterfaces.clear();
    for ( int i = 0 ; i < n ; i++ )
        addInterface( tr("LAN%1").arg(i+1) );
}


