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


RouterDevice::RouterDevice(QObject *parent) : SmartDevice(parent)
{        
}

RouterDevice* RouterDevice::create(QObject *parent)
{
    RouterDevice *r = new RouterDevice(parent);
    r->setSocketsCount( appSetting::defaultRouterCount() );
    r->setRouteModel( new RouteModel() );
    r->setNote(tr("<b>Router</b><!--You can use HTML.-->"));
    return r;
}

void RouterDevice::dialog()
{
    routerProperty *d = new routerProperty;
    routerSetting *set = new routerSetting(this);
    d->setRouter(set);
    d->exec();
    delete d;
    delete set;
}

void RouterDevice::write(QDataStream &stream) const
{
    stream << routerDev;
    SmartDevice::write(stream);
}

void RouterDevice::read(QDataStream &stream)
{
   SmartDevice::read(stream);
}

void RouterDevice::setSocketsCount(int n)
{ 
    int t = myInterfaces.size();
    if ( t <= n ) {
        for ( int i = t; i < n ; i++ )
            addInterface( tr("LAN%1").arg(i+1) );
    }
    else {
        foreach ( Interface *i , myInterfaces )
            if ( i->isConnect() ) {
                QMessageBox::warning(0,tr("Error"), tr("To change the number of ports, disconnect all cables!"),
                                     QMessageBox::Ok , QMessageBox::Ok);
                return;
            }
        for ( int i = t-1 ; i >= n ; i-- ) {
            delete myInterfaces[i];
            myInterfaces.pop_back();
        }
    }
}


