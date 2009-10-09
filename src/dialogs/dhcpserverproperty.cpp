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

dhcpServerProperty::dhcpServerProperty(QWidget *parent) : QDialog(parent)
{
    setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    begin = new ipEdit(tr("From "));
    begin->setEnabled(false);
    end = new ipEdit(tr("to"));
    end->setEnabled(false);
    mask = new ipEdit(tr("Mask"));
    mask->setEnabled(false);
    gatew = new ipEdit(tr("Gateway"));
    gatew->setEnabled(false);
    QHBoxLayout *l = new QHBoxLayout;
    l->addWidget(begin);
    l->addWidget(end);
    lay->addLayout(l);
    l = new QHBoxLayout;
    l->addWidget(mask);
    l->addWidget(gatew);
    lay->addLayout(l);
    QStringList list;
    list << tr("Mac") << tr("Ip") << tr("Mask") << tr("Gateway");
    tw_static->setHorizontalHeaderLabels(list);    
}

void dhcpServerProperty::setProgramm(dhcpServerProgramm *prog)
{
    myProgramm = prog;
}

void dhcpServerProperty::addRecord()
{
    tw_static->insertRow(tw_static->rowCount());
}

void dhcpServerProperty::deleteRecord()
{
    tw_static->removeRow(tw_static->currentRow());
}

void dhcpServerProperty::changeState(bool b)
{
    begin->setEnabled(b);
    end->setEnabled(b);
    mask->setEnabled(b);
    gatew->setEnabled(b);
}

void dhcpServerProperty::apply()
{
    for ( int i = 0; i < tw_static->rowCount(); i++ ) {
        staticRecord *rec = new staticRecord;
        rec->chaddr = macAddress(tw_static->item(i,0)->text());
        rec->yiaddr = ipAddress(tw_static->item(i,1)->text());
        rec->mask = ipAddress(tw_static->item(i,2)->text());
        rec->gateway = ipAddress(tw_static->item(i,3)->text());
        rec->time = sb_time->value();
        if ( !myProgramm->containRecord(rec) ) myProgramm->addStaticRecord(rec);
    }
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
