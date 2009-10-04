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

computer::computer(int c /* = 0 */)
{
    if ( !c ) c = appSetting::defaultComputerCount();
    for ( int i = 0 ; i < c ; i++)
        addInterface(tr("eth%1").arg(i));
    myRouteTable->addToTable("127.0.0.0","255.0.0.0","127.0.0.1","127.0.0.1",0,routeModel::connectMode);
    setNote(tr("<b>Computer</b><!--You can use HTML.-->"));
}

void computer::showDesignerDialog()
{
    designerDialog *d = new designerDialog(this);
    d->show();
}

void computer::dialog()
{
#ifndef __TESTING__
    computerProperty *d = new computerProperty;
    d->setDevice( new computerSetting(this) );
    d->exec();
#endif
}

void computer::sendConstructedFrame(QString Interface, frame Frame, int count)
{
    for ( int i = 0 ; i < count ; i++ )
        adapter(Interface)->pushToSocket(Frame);
}



