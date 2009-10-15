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
#include "computerproperty.h"
#include "computer.h"
#include "ipedit.h"
#include <QCheckBox>
#include <QLabel>
#include <QPlainTextEdit>

computerProperty::computerProperty()
{
    setWindowTitle(tr("Properties"));
    QVBoxLayout *all = new QVBoxLayout;
    QVBoxLayout *prop = new QVBoxLayout;
    QHBoxLayout *temp = new QHBoxLayout;
    gateway = new ipEdit(this,tr("Default gateway:"));
    connect( gateway , SIGNAL(textChanged(QString)) , SLOT(applyEnable()) );
    temp->addWidget(gateway,0,Qt::AlignLeft);
    prop->addLayout(temp);
    cb_route = new QCheckBox(tr("Enable routing"));
    connect( cb_route , SIGNAL(stateChanged(int)) , SLOT(applyEnable()) );
    prop->addWidget(cb_route);
    te_text = new QPlainTextEdit;
    connect( te_text , SIGNAL(textChanged()) , SLOT(applyEnable()) );
    te_text->setFixedHeight(100);
    te_text->setMaximumBlockCount(5);
    prop->addWidget( new QLabel(tr("Description:")));
    prop->addWidget(te_text);
    prop->addStretch(1);
    all->addLayout(prop);
    all->addLayout(lay);
    setLayout(all);
    setAttribute(Qt::WA_DeleteOnClose);
}

computerProperty::~computerProperty()
{
    delete comp;
}

void computerProperty::setDevice(computerSetting *c)
{
    comp = c;
    gateway->setText( c->gateway() );
    cb_route->setChecked( comp->isRouter() );
    btn_apply->setEnabled(false);
    te_text->setPlainText( comp->note() );
}

void computerProperty::apply()
{
    if (gateway->text() != "0.0.0.0" )
    comp->setGateway(gateway->text());
    comp->setRouter( cb_route->isChecked() );
    comp->setNote( te_text->toPlainText() );
    if ( sender() == btn_ok ) accept();
}

