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
#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtGui/QLineEdit>
#include <QtGui/QMessageBox>
#include <QtGui/QCheckBox>
#include "switchproperty.h"
#include "switchdevice.h"
#include "ipedit.h"

switchProperty::switchProperty()
{
    setWindowTitle(tr("Properties"));
    QVBoxLayout *all = new QVBoxLayout;
    QHBoxLayout *temp = new QHBoxLayout;
    QLabel *t = new QLabel(tr("Number of ports: "));
    cb_count = new QComboBox;
    QStringList s;
    s << "4" << "5" << "8" << "12" <<"16"<< "24"<<"32" << "48";
    cb_count->addItems(s);
    connect( cb_count , SIGNAL(currentIndexChanged(int)) ,SLOT(applyEnable()) );
    temp->addWidget(t);
    temp->addWidget(cb_count);
    all->addLayout(temp);

    chb_manual = new QCheckBox(tr("Manageable"));
    connect( chb_manual , SIGNAL(clicked()) , SLOT(applyEnable()) );
    all->addWidget(chb_manual);

    temp = new QHBoxLayout;
    lb_mac = new QLabel(tr("Mac-address: "));
    le_mac = new QLineEdit;
    le_mac->setInputMask("HH:HH:HH:HH:HH:HH;_");
    temp->addWidget(lb_mac);
    temp->addWidget(le_mac);
    all->addLayout(temp);
    connect( le_mac , SIGNAL(textChanged(QString)) , SLOT(applyEnable()));

    le_ip = new ipEdit(this,tr("Ip-address: "));
    all->addWidget(le_ip);
    connect( le_ip , SIGNAL(textChanged(QString)) , SLOT(applyEnable()) );

    le_mask = new ipEdit(this,tr("Mask: "));
    all->addWidget(le_mask);
    connect( le_mask , SIGNAL(textChanged(QString)) , SLOT(applyEnable()));

    lb_statics = new QLabel;
    all->addWidget(lb_statics);

    QPushButton *btn_reset = new QPushButton(QIcon(":/im/images/refresh.png"),tr("Reset statistics"));
    connect( btn_reset , SIGNAL(clicked()), SLOT(reset()) );
    all->addWidget(btn_reset , 0 , Qt::AlignRight );

    all->addStretch(1);
    all->addLayout(lay);
    setLayout(all);
    setAttribute( Qt::WA_DeleteOnClose );
}

switchProperty::~switchProperty()
{
    delete sw;
}

void switchProperty::setSwitch(boxSetting *d)
{
    sw = d;
    cb_count->setCurrentIndex( cb_count->findText( QString::number( d->socketsCount() ) ) );
    le_mac->setText( d->snmpMac() );
    le_ip->setText( d->snmpIp() );
    le_mask->setText( d->snmpMask() );
    lb_statics->setText( d->statics() );
    chb_manual->setChecked( d->isManual() );
    btn_apply->setEnabled(false);
}

void switchProperty::reset()
{
    sw->reset();
    setSwitch(sw);
}

void switchProperty::apply()
{
    int t = sw->socketsCount();
    if ( t != cb_count->currentText().toInt() )
        if ( !sw->setSocketsCount( cb_count->currentText().toInt() ) ) {
            QMessageBox::warning(this,tr("Error"),
            tr("First, remove the cables!") , QMessageBox::Ok , QMessageBox::Ok);
            cb_count->setCurrentIndex( cb_count->findText( QString::number(t) ) );
            return;
        }
    sw->setMac(le_mac->text());
    sw->setIp(le_ip->text());
    sw->setMask(le_mask->text());
    sw->setManual( chb_manual );
    if ( sender() == btn_ok ) accept();
}



