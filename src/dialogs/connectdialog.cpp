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
#include "connectdialog.h"
#include "device.h"
#include "interface.h"

connectDialog::connectDialog(device *s, device *e)
{
    setupUi(this);
    start = s;
    end = e;
    connect( startList , SIGNAL(itemSelectionChanged()) , SLOT(changeSelect()));
    connect( endList , SIGNAL(itemSelectionChanged()) , SLOT(changeSelect()));

    foreach ( QString i , start->sockets() )
         new QListWidgetItem( connectIcon( start->isConnectSocket(i) ) , i , startList, start->isConnectSocket(i) );

    foreach ( QString i , end->sockets() )
        new QListWidgetItem( connectIcon( end->isConnectSocket(i) ) , i , endList, end->isConnectSocket(i) );

    for ( int i = 0 ; i < startList->count() ; i++ )
        if ( !startList->item(i)->type() ) {
            startList->setCurrentRow(i);
            break;
        }

    for ( int i = 0 ; i < endList->count() ; i++ )
        if ( !endList->item(i)->type() ) {
            endList->setCurrentRow(i);
            break;
        }
    resize( sizeHint());
}

void connectDialog::changeSelect()
{
    if ( startList->currentRow() < 0 || endList->currentRow() < 0 ) {
        okButton->setEnabled(false);
        return;
    }
    okButton->setEnabled( !start->isConnectSocket( startList->currentItem()->text() ) &&
                          !end->isConnectSocket( endList->currentItem()->text() ) );
}

void connectDialog::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

QIcon connectDialog::connectIcon(bool b) const
{
    if (b) return QIcon(":/im/images/ok.png");
    else return QIcon(":/im/images/not.png");
}



