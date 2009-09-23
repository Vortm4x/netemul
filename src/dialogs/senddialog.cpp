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
#include <QPushButton>
#include <QRadioButton>
#include <QGroupBox>
#include <QLabel>
#include <QListWidget>
#include <QSlider>
#include <QSpinBox>
#include <QBoxLayout>
#include <QtDebug>
#include <QCheckBox>
#include <QMessageBox>
#include "senddialog.h"
#include "device.h"


sendDialog::sendDialog(aim cur,device* t)
{
    myState = cur;
    myDevice = t->contentDevice();
    setWindowTitle(tr("Sending"));
    all = new QVBoxLayout;
    prepare();
    QHBoxLayout *temp = new QHBoxLayout;
    temp->addWidget(okButton);
    temp->addWidget(cancelButton);
    all->addLayout(temp);
    setLayout(all);
    connect( okButton , SIGNAL(clicked()) , this , SLOT(checkAccept()));
    connect( cancelButton , SIGNAL(clicked()) , this , SLOT(reject()));
// НЕ НАДО ДЕЛАТЬ setAttribute(Qt::WA_DeleteOnClose); !!!
}

void sendDialog::checkSelected(int cur)
{
    okButton->setEnabled( cur >= 0 );
}

void sendDialog::checkAccept()
{
    if (myState == sender) {
        mySize = sizeSlider->value();
        myBroadcast = check->checkState();
        if ( rtn_tcp->isChecked() ) myProtocol = TCP;
        else myProtocol = UDP;
    } else {
        myDest = myDevice->nameToIp( list->currentItem()->text() );
    }
    accept();
}

void sendDialog::prepare()
{
    cancelButton = new QPushButton(QIcon(":/im/images/not.png"),tr("Cancel"));
    if ( myState == sender ) {
        sizeSlider = new QSlider(Qt::Horizontal);
        sizeCaption = new QLabel(tr("Size KB"));
        sizeBox = new QSpinBox;
        sizeSlider->setRange(1,1000);
        sizeBox->setRange(1,1000);
        sizeBox->setSuffix(tr(" KB"));
        connect( sizeBox, SIGNAL(valueChanged(int)),sizeSlider, SLOT(setValue(int)));
        connect(sizeSlider , SIGNAL(valueChanged(int)) ,sizeBox, SLOT(setValue(int)));
        sizeBox->setValue(50);
        check = new QCheckBox(tr("Broadcast"));
        check->setChecked(false);
        check->setEnabled(false);
        okButton = new QPushButton(QIcon(":/im/images/ok.png"),tr("Next"));
        QGroupBox *box = new QGroupBox(tr("Choose protocol:"));
        QVBoxLayout *btn = new QVBoxLayout;
        rtn_udp = new QRadioButton(tr("UDP"));
        rtn_udp->setChecked(true);
        rtn_tcp = new QRadioButton(tr("TCP"));        
        rtn_tcp->setEnabled(true);
        btn->addWidget(rtn_udp);
        btn->addWidget(rtn_tcp);
        box->setLayout(btn);
        all->addWidget(box);
        all->addWidget(sizeCaption);
        QHBoxLayout *temp = new QHBoxLayout;
        temp->addWidget(sizeSlider);
        temp->addWidget(sizeBox);
        all->addLayout(temp);
        all->addWidget(check);
        all->addStretch(1);
    }
    else {
        list = new QListWidget;
        QStringList portList = myDevice->sockets();
        foreach ( QString i , portList)
            if ( myDevice->isConnectSocket(i) ) {
                QListWidgetItem *temp = new QListWidgetItem( QIcon(":/im/images/ok.png") , i);
                list->addItem(temp);
            }
        caption = new QLabel(tr("Select the network card receiver"));
        okButton = new QPushButton(QIcon(":/im/images/ok.png"),tr("Send"));
        if (list->count()) {
            okButton->setEnabled(true);
            list->setCurrentRow(0);
        }
        else okButton->setEnabled(false);
        all->addWidget(caption);
        all->addWidget(list);
        connect( list , SIGNAL(currentRowChanged(int)) ,SLOT(checkSelected(int)));
    }
}

