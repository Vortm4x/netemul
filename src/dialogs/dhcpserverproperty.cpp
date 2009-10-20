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
#include "smartdevice.h"

dhcpServerProperty::dhcpServerProperty(smartDevice *dev,QWidget *parent /* = 0 */) : QDialog(parent)
{    
    setupUi(this);
    device = dev;
    setAttribute(Qt::WA_DeleteOnClose);
    QStringList list;
    list << tr("Mac") << tr("Ip") << tr("Mask") << tr("Gateway");
    tw_static->setHorizontalHeaderLabels(list);
    tw_static->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    foreach ( interface *i, device->interfaces() )
        if ( i->isConnect() ) cb_interface->addItem( QIcon(":im/images/ok.png"), i->name() );
}

void dhcpServerProperty::setProgramm(dhcpServerProgramm *prog)
{
    myProgramm = prog;
    foreach ( staticRecord *i, myProgramm->statics() ) {
        tw_static->insertRow( tw_static->rowCount() );

    }
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
    ie_begin->setEnabled(b);
    ie_end->setEnabled(b);
    ie_mask->setEnabled(b);
    ie_gatew->setEnabled(b);
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
        myProgramm->setInterface( cb_interface->currentText() );
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
