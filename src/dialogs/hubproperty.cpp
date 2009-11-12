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
#include "hubproperty.h"
#include "hubdevice.h"
#include "ipedit.h"

hubProperty::hubProperty()
{
    setWindowTitle(tr("Properties"));
    QVBoxLayout *all = new QVBoxLayout;
    QHBoxLayout *temp = new QHBoxLayout;
    QLabel *t = new QLabel(tr("Number of ports: "));
    cb_count = new QComboBox;
    QStringList s;
    s << "4" << "5" << "6" << "8" << "12" << "16" << "24" << "48";
    cb_count->addItems(s);
    connect( cb_count , SIGNAL(currentIndexChanged(int)) ,SLOT(applyEnable()) );
    temp->addWidget(t);
    temp->addWidget(cb_count);
    all->addLayout(temp);

    temp = new QHBoxLayout;
    lb_mac = new QLabel(tr("Mac-address: "));
    le_mac = new QLineEdit;
    le_mac->setInputMask("HH:HH:HH:HH:HH:HH;_");
    temp->addWidget(lb_mac);
    temp->addWidget(le_mac);
    connect( le_mac , SIGNAL(textChanged(QString)) , SLOT(applyEnable()));
    all->addLayout(temp);

    lb_statics= new QLabel;
    all->addWidget(lb_statics);

    lb_conflict = new QLabel;
    all->addWidget( lb_conflict );

    QPushButton *btn_reset = new QPushButton(QIcon(":/im/images/refresh.png"),tr("Reset statistics"));
    connect( btn_reset , SIGNAL(clicked()), SLOT(reset()) );
    all->addWidget(btn_reset , 0 , Qt::AlignRight );

    all->addStretch(1);
    all->addLayout(lay);
    setLayout(all);
    setAttribute(Qt::WA_DeleteOnClose);
}

hubProperty::~hubProperty()
{
}

void hubProperty::setHub(hubSetting *s)
{
    st = s;
    cb_count->setCurrentIndex( cb_count->findText( QString::number(st->socketsCount() ) ));
    lb_conflict->setText( tr("Number of collisions: %1").arg( st->collisions() ) );
    le_mac->setText( st->snmpMac() );
    lb_statics->setText( st->statics() );
    btn_apply->setEnabled(false);
}

void hubProperty::reset()
{
    st->reset();
    setHub(st);
}

void hubProperty::apply()
{
    int t = st->socketsCount();
    if ( t != cb_count->currentText().toInt() )
        if ( !st->setSocketsCount( cb_count->currentText().toInt() ) ) {
            QMessageBox::warning(this,tr("Error"),
            tr("First, remove the cables!") , QMessageBox::Ok , QMessageBox::Ok);
            cb_count->setCurrentIndex( cb_count->findText( QString::number(t) ) );
            return;
        }
    st->setMac(le_mac->text());
    if ( sender() == btn_ok ) accept();
}

