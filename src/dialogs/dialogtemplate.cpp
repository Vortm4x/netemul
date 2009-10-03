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
#include "dialogtemplate.h"
#include <QApplication>
#include <QStyle>

dialogTemplate::dialogTemplate()
{    
    lay = new QHBoxLayout;
    btn_cancel = new QPushButton(QIcon(qApp->style()->standardIcon( QStyle::SP_DialogCancelButton)),
                                 tr("Cancel"));
    connect( btn_cancel , SIGNAL(clicked()) , SLOT(reject()) );
    btn_ok = new QPushButton(QIcon(qApp->style()->standardIcon( QStyle::SP_DialogOkButton)),
                             tr("Ok"));
    connect( btn_ok , SIGNAL(clicked()) , SLOT(apply()) );
    btn_apply = new QPushButton(QIcon(qApp->style()->standardIcon( QStyle::SP_DialogApplyButton)),
                                tr("Apply"));
    connect(btn_apply, SIGNAL(clicked()) , SLOT(applyDisable()));

    lay->addWidget(btn_apply);
    lay->addWidget(btn_ok);
    lay->addWidget(btn_cancel);
    lay->setAlignment(Qt::AlignRight);
}

void dialogTemplate::applyEnable()
{
    btn_apply->setEnabled(true);
}

void dialogTemplate::applyDisable()
{
    btn_apply->setEnabled(false);
    apply();
}
