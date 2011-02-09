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
#include "computer.h"
#include "interface.h"
#include "routemodel.h"
#include "appsetting.h"
#ifndef __TESTING__
#include "computerproperty.h"
#include "designerdialog.h"
#endif

#include <QMessageBox>

Computer::Computer(QObject *parent) : SmartDevice(parent)
{    
}

Computer* Computer::create(QObject *parent)
{
    Computer *c = new Computer(parent);
    c->setSocketsCount(AppSetting::defaultComputerCount());
    c->setRouteModel( new RouteModel(c) );
    c->routeModel()->addToTable("127.0.0.0","255.0.0.0","127.0.0.1","127.0.0.1",0,RouteModel::connectMode);
    c->setNote(tr("<b>Computer</b><!--You can use HTML.-->"));
    return c;
}

void Computer::showDesignerDialog()
{
    designerDialog *d = new designerDialog(this);
    d->show();
}

void Computer::dialog()
{
    computerProperty *d = new computerProperty;
    d->setDevice( new ComputerSetting(this) );
    d->exec();
}

void Computer::setSocketsCount(int n)
{
    int t = myInterfaces.size();
    if ( t <= n ) {
        for ( int i = t; i < n ; i++ )
            addInterface( tr("eth%1").arg(i) );
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

void Computer::sendConstructedFrame(const QString &interface, Frame fr, int count)
{
    for ( int i = 0 ; i < count ; i++ )
        adapter(interface)->pushToSocket(fr);
}

void Computer::setRouteModel(RouteModel *model)
{
    SmartDevice::setRouteModel(model);
    model->addToTable("127.0.0.0","255.0.0.0","127.0.0.1","127.0.0.1",0,RouteModel::connectMode);
}



