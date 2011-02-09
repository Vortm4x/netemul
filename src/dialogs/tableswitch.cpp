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
#include "tableswitch.h"
#include "switchdevice.h"
#include "switchmodel.h"

TableSwitch::TableSwitch(SwitchTableSetting *s)
{
    sw = s;
    t_sw = s->switchTable();
    setupUi(this);
    table->setModel(t_sw);
    QHeaderView *h = table->horizontalHeader();
    h->setResizeMode( QHeaderView::Stretch );
    int n = s->socketsCount();
    for ( int i = 0 ; i < n ; i++ )
        cb_port->addItem(tr("LAN%1").arg(i+1));
    connect( btn_add , SIGNAL(clicked()) , SLOT(addRecord()) );
    connect( btn_del , SIGNAL(clicked()) , SLOT(deleteRecord()) );
    connect( table->selectionModel() , SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(selectionChanged()) );
    connect( t_sw , SIGNAL(modelReset()) , SLOT(resetSelection()));
}

void TableSwitch::addRecord()
{    
    t_sw->addToTable( macAddress( le_macAddress->text() ) , cb_port->currentText()  , SwitchModel::staticMode);
    le_macAddress->setText("00:00:00:00:00:00");
}

void TableSwitch::deleteRecord()
{
    QItemSelectionModel *m = table->selectionModel();
    QModelIndexList l = m->selectedIndexes();
    foreach ( QModelIndex i , l )
        if ( i.column() == 0 ) {
            t_sw->deleteFromTable( i.data(Qt::DisplayRole).toString() );
            return;
        }
}

void TableSwitch::resetSelection()
{
    btn_del->setEnabled(false);
}

void TableSwitch::selectionChanged()
{
    btn_del->setEnabled(true);
}

void TableSwitch::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
