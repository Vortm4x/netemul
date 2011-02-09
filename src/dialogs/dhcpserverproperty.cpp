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
#include <QMessageBox>
#include "dhcpserverproperty.h"
#include "dhcpserverprogramm.h"
#include "dhcpservermodel.h"
#include "smartdevice.h"
#include "macaddressdelegate.h"
#include "ipaddressdelegate.h"

DhcpServerProperty::DhcpServerProperty(SmartDevice *dev,QWidget *parent /* = 0 */) : QDialog(parent)
{
    setupUi(this);
    device = dev;
    setAttribute(Qt::WA_DeleteOnClose);
    foreach ( Interface *i, device->interfaces() )
        if ( i->isConnect() ) cb_interface->addItem( QIcon(":im/images/ok.png"), i->name() );
    macDelegate = new MacAddressDelegate(this);
    ipDelegate = new IpAddressDelegate(this);
    tv_static->setItemDelegateForColumn(0, macDelegate );
    for ( int i = 1 ; i <= 3 ; i++ )
        tv_static->setItemDelegateForColumn(i,ipDelegate);
    connect( ie_begin , SIGNAL(maskChanged(quint8)) , ie_mask , SLOT(setDefaultMask(quint8)) );
}

DhcpServerProperty::~DhcpServerProperty()
{
    delete macDelegate;
}

void DhcpServerProperty::setProgramm(DhcpServerProgram *prog)
{
//    myProgramm = prog;
//    myModel = myProgramm->dhcpModel();
//    tv_static->setModel( myModel );
//    QHeaderView *h = tv_static->horizontalHeader();
//    h->setResizeMode( QHeaderView::Stretch );
//    cb_dynamic->setChecked(myProgramm->dynamic());
//    sb_time->setValue(myProgramm->time());
//    ie_begin->setText(myProgramm->beginIp());
//    ie_end->setText(myProgramm->endIp());
//    ie_mask->setText(myProgramm->mask());
//    ie_gatew->setText(myProgramm->gateway());
//    cb_interface->setCurrentIndex( cb_interface->findText(myProgramm->interfaceName() ));
//    sb_waitingTime->setValue( myProgramm->waitingTime() );
//    le_name->setText(myProgramm->serverName());
}

void DhcpServerProperty::addRecord()
{
    myModel->insertRow( myModel->rowCount() );
}

void DhcpServerProperty::deleteRecord()
{
    myModel->removeRow( tv_static->currentIndex().row() );
}

void DhcpServerProperty::changeState(bool b)
{
    ie_begin->setEnabled(b);
    ie_end->setEnabled(b);
    ie_mask->setEnabled(b);
    ie_gatew->setEnabled(b);
    sb_time->setEnabled(b);
    lb_term->setEnabled(b);
}

void DhcpServerProperty::apply()
{
    if ( ie_begin->ipText() > ie_end->ipText() ) {
        QMessageBox::warning(0,tr("Wrong range"),tr("You have entered a wrong IP range."), QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
//    myProgramm->setInterfaceName(cb_interface->currentText());
//    myProgramm->setBeginIp(ie_begin->ipText().toString());
//    myProgramm->setEndIp(ie_end->ipText().toString());
//    myProgramm->setMask(ie_mask->ipText().toString());
//    myProgramm->setGateway(ie_gatew->ipText().toString());
//    myProgramm->setTime(sb_time->value());
//    myProgramm->setDynamic(cb_dynamic->isChecked());
//    myProgramm->setWaitingTime(sb_waitingTime->value());
//    myProgramm->setServerName(le_name->text());
    accept();
}

void DhcpServerProperty::changeEvent(QEvent *e)
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
