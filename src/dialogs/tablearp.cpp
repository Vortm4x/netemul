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
#include "tablearp.h"
#include "ipedit.h"
#include "arpmodel.h"
#include "smartdevice.h"
#include <QPushButton>
#include <QTableWidget>
#include <QLineEdit>
#include <QBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QHeaderView>

/*!
   Конструктор создает основной интерфейс окна.
*/
tableArp::tableArp()
{
    setWindowTitle(tr("Arp table"));
    QVBoxLayout *all = new QVBoxLayout;
    table = new QTableWidget(0,5);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setMinimumWidth(300);
    table->setAlternatingRowColors(true);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QStringList head;
    head << tr("Mac-address") << tr("Ip-address") << tr("Record type") << tr("Netcard name")
        << tr("TTL") ;
    table->setHorizontalHeaderLabels(head);
    table->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    all->addWidget(table);
    QHBoxLayout *temp = new QHBoxLayout;
    QLabel *lb = new QLabel(tr("Mac-address: "));
    temp->addWidget(lb);
    le_mac = new QLineEdit;
    le_mac->setFixedWidth(150);
    le_mac->setInputMask("HH:HH:HH:HH:HH:HH;_");
    le_mac->setText("00:00:00:00:00:00");
    temp->addWidget(le_mac);
    ip = new ipEdit(this,tr("Ip-address: "));
    temp->addWidget(ip);
    lb = new QLabel( tr("Netcard") );
    cb_port = new QComboBox;
    cb_port->setFixedWidth(100);    
    temp->addWidget(lb);
    temp->addWidget(cb_port);
    all->addLayout(temp);
    temp = new QHBoxLayout;
    btn_add = new QPushButton( QIcon(":im/images/edit_add.png"), tr( "Add" ));
    connect(btn_add, SIGNAL(clicked()), SLOT(addRecord()));
    btn_del = new QPushButton( QIcon( ":im/images/edit_remove.png" ), tr( "Delete" ));
    connect(btn_del, SIGNAL(clicked()), SLOT(deleteRecord()));
    btn_del->setEnabled(false);
    btn_close = new QPushButton( QIcon( ":im/images/not.png" ), tr( "Close" ));
    connect(btn_close, SIGNAL(clicked()), SLOT(reject()));
    connect( table , SIGNAL(itemSelectionChanged()) , SLOT(checkSelection()));
    temp->addWidget(btn_add);
    temp->addWidget(btn_del);
    temp->addStretch(1);
    temp->addWidget(btn_close);
    all->addLayout(temp);
    setLayout(all);
    setAttribute(Qt::WA_DeleteOnClose);
}
//------------------------------------------------------------------
void tableArp::setDevice(SmartDevice *dev)
{
    device = dev;
    s = device->sockets();    
    list = device->arpModels();
    updateTable();
}
/**
  Обновляет содержимое таблицы арп записей при каких либо изменениях.
*/
void tableArp::updateTable()
{
    table->clearContents();
    cb_port->clear();
    table->setRowCount(0);
    int i = 0;
    ArpRecord *r;
    foreach ( ArpModel *p, list ) {
        if ( !device->isConnectSocket(s.at(i)) ) { i++; continue; }
        cb_port->addItem(QIcon(":im/images/ok.png"), s.at(i) );
        for ( int j = 0; j < p->size(); j++ ) {
            r = p->recordAt(j);
            table->insertRow(table->rowCount());
            addItem( table->rowCount()-1, 0, r->mac.toString());
            addItem( table->rowCount()-1, 1, r->ip.toString());
            addItem( table->rowCount()-1, 2, r->modeString());
            addItem( table->rowCount()-1, 3, s.at(i));
            addItem( table->rowCount()-1, 4, QString::number(r->time));
        }
        i++;
    }
    btn_del->setEnabled(false);
}
//---------------------------------------------------------------
/**
  Добавляет в таблицу arp указанного адаптера статическую запись.
  Если значение mac или ip пустые добавления не происходит, обновляет таблицу и
  обнуляет поля для ввода.
*/
void tableArp::addRecord()
{
    if ( le_mac->text() == "00:00:00:00:00:00" || ip->text() == "0.0.0.0" ) return;
    list.at(s.indexOf(cb_port->currentText()))->addToTable( ip->text(), le_mac->text(), ArpModel::staticMode );
    updateTable();
    ip->setText("0.0.0.0");
    le_mac->setText("00:00:00:00:00:00");
}
//----------------------------------------------------------------------
/**
  Удалеяет из таблицы существующую запись и обновляет таблицу.
*/
void tableArp::deleteRecord()
{    
    if ( table->selectedItems().isEmpty() ) return;
    int n = table->currentRow();
    list.at(s.indexOf( table->item( n, 3)->text() ))->deleteFromTable( table->item( n, 1)->text() );
    updateTable();
}
//------------------------------------------------------------------------

void tableArp::checkSelection()
{
    if ( table->currentRow() == -1 ) btn_del->setEnabled(false);
    else btn_del->setEnabled(true);
}

void tableArp::addItem(int r, int c, QString s)
{
    QTableWidgetItem *item = new QTableWidgetItem( s );
    table->setItem( r, c, item);
}
