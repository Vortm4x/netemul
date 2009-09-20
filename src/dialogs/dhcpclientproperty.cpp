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
#include "dhcpclientproperty.h"
#include "dhcpclientprogramm.h"

dhcpClientProperty::dhcpClientProperty(QWidget *parent) : QDialog(parent)
{
    setupUi(this);
    setAttribute( Qt::WA_DeleteOnClose );
}

void dhcpClientProperty::setProgramm(dhcpClientProgramm *prog)
{
    myProgramm = prog;
    foreach ( QString i , myProgramm->interfacesList() ) {
        QListWidgetItem *item = new QListWidgetItem( myProgramm->isConnectSocketIcon(i), i , lw_interfaces );
        item->setCheckState( myProgramm->checkedState( i ) );
    }
}

void dhcpClientProperty::apply()
{
    for ( int i = 0 ; i < lw_interfaces->count() ; i++ ) {
        QListWidgetItem *t = lw_interfaces->item(i);
        myProgramm->observeInterface( t->text() , t->checkState() == Qt::Checked );
    }
    accept();
}

void dhcpClientProperty::changeEvent(QEvent *e)
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
