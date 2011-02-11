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
//#include <QApplication>
//#include <QStyle>
#include "dhcpserverproperty.h"
#include "dhcpservermodel.h"
#include "smartdevice.h"
#include "macaddressdelegate.h"
#include "ipaddressdelegate.h"

DhcpServerProperty::DhcpServerProperty(DhcpServerSetting *setting, QWidget *parent) : QDialog(parent)
{
    setupUi(this);
    myPortMap = new QMap<int, Interface*>;
    mySetting = setting;
    SmartDevice *dev = mySetting->program()->device();
    setAttribute(Qt::WA_DeleteOnClose);
    btn_apply->setIcon(QIcon(qApp->style()->standardIcon( QStyle::SP_DialogApplyButton)));
    port_tb = new QTabBar;
    tab_lay->addWidget(port_tb);
    foreach ( Interface *i, dev->interfaces() ) {
        if ( i->isConnect() ) {
            int index = port_tb->addTab(QIcon(":im/images/ok.png"), i->name() );
            myPortMap->insert(index, i);
        }
    }
    connect(port_tb, SIGNAL(currentChanged(int)), SLOT(onCurrentChanged(int)));
    port_tb->setCurrentIndex(0);
    onCurrentChanged(0);
    macDelegate = new MacAddressDelegate(this);
    ipDelegate = new IpAddressDelegate(this);
    tv_static->setItemDelegateForColumn(0, macDelegate );
    for ( int i = 1 ; i <= 3 ; i++ )
        tv_static->setItemDelegateForColumn(i,ipDelegate);
    connect( ie_begin , SIGNAL(maskChanged(quint8)) , ie_mask , SLOT(setDefaultMask(quint8)) );
}

DhcpServerProperty::~DhcpServerProperty()
{
    myPortMap->clear();
    delete mySetting;
    delete macDelegate;
    delete ipDelegate;
}

//----------Private functions--------------
void DhcpServerProperty::clearForm()
{
    tv_static->clearSpans();
    cb_turnOn->setChecked(false);
    cb_dynamic->setChecked(false);
    sb_time->cleanText();
    ie_begin->clear();
    ie_end->clear();
    ie_gatew->clear();
    ie_mask->clear();
    sb_waitingTime->cleanText();
}

//----------Private slots--------------
void DhcpServerProperty::onCurrentChanged(int index)
{
    myDaemon = mySetting->daemonOf(myPortMap->value(index));
    if ( myDaemon == NULL ) clearForm();
    myModel = myDaemon->dhcpModel();
    cb_turnOn->setChecked(myDaemon->isTurnOn());
    onTurnedOnOff(cb_turnOn->isChecked());
    tv_static->setModel(myModel);
    QHeaderView *h = tv_static->horizontalHeader();
    h->setResizeMode( QHeaderView::Stretch );
    cb_dynamic->setChecked(myDaemon->dynamic());
    sb_time->setValue(myDaemon->time());
    ie_begin->setText(myDaemon->beginIp());
    ie_end->setText(myDaemon->endIp());
    ie_gatew->setText(myDaemon->gateway());
    ie_mask->setText(myDaemon->mask());
    sb_waitingTime->setValue(myDaemon->waitingTime());
}

void DhcpServerProperty::onTurnedOnOff(bool b)
{
    static_lb->setEnabled(b);
    tv_static->setEnabled(b);
    btn_add->setEnabled(b);
    btn_del->setEnabled(b);
    waiting_lb->setEnabled(b);
    sb_waitingTime->setEnabled(b);
    cb_dynamic->setEnabled(b);
}

void DhcpServerProperty::addRecord()
{
    if ( myModel == NULL ) return;
    myModel->insertRow( myModel->rowCount() );
}

void DhcpServerProperty::deleteRecord()
{
    if ( myModel == NULL ) return;
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
    if ( ie_begin->ipAddress() > ie_end->ipAddress() ) {
        QMessageBox::warning(0,tr("Wrong range"),tr("You have entered a wrong IP range."), QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    myDaemon->setTurnOn(cb_turnOn->isChecked());
    myDaemon->setBeginIp(ie_begin->ipAddress());
    myDaemon->setEndIp(ie_end->ipAddress());
    myDaemon->setDynamic(cb_dynamic->isChecked());
    myDaemon->setGateway(ie_gatew->ipAddress());
    myDaemon->setMask(ie_mask->ipAddress());
    myDaemon->setTime(sb_time->value());
    myDaemon->setWaitingTime(sb_waitingTime->value());
}

void DhcpServerProperty::onOkButtonClicked()
{
    apply();
    accept();
}

//-----------------------------------------

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
