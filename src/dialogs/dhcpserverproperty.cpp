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
#include "dhcpserverproperty.h"
#include "dhcpserverprogramm.h"
#include "dhcpservermodel.h"
#include "smartdevice.h"

dhcpServerProperty::dhcpServerProperty(smartDevice *dev,QWidget *parent /* = 0 */) : QDialog(parent)
{    
    setupUi(this);
    device = dev;
    setAttribute(Qt::WA_DeleteOnClose);
    foreach ( interface *i, device->interfaces() )
        if ( i->isConnect() ) cb_interface->addItem( QIcon(":im/images/ok.png"), i->name() );
}

void dhcpServerProperty::setProgramm(dhcpServerProgramm *prog)
{
    myProgramm = prog;
    myModel = myProgramm->dhcpModel();
    tv_static->setModel( myModel );
}

void dhcpServerProperty::addRecord()
{
    myModel->insertRow( myModel->rowCount() );
}

void dhcpServerProperty::deleteRecord()
{
    myModel->removeRow( tv_static->currentIndex().row() );
}

void dhcpServerProperty::changeState(bool b)
{
    ie_begin->setEnabled(b);
    ie_end->setEnabled(b);
    ie_mask->setEnabled(b);
    ie_gatew->setEnabled(b);
}

void dhcpServerProperty::apply()
{
    accept();
}

void dhcpServerProperty::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
