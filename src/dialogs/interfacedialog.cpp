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
#include "interfacedialog.h"
#include "interface.h"
#include <QPushButton>
#include <QLabel>
#include <QBoxLayout>
#include <QListWidget>
#include <QLineEdit>

interfaceDialog::interfaceDialog()
{
    setWindowTitle(tr("Add adapter"));
    QHBoxLayout *all = new QHBoxLayout;
    QVBoxLayout *temp = new QVBoxLayout;
    QVBoxLayout *tempL = new QVBoxLayout;

    picture = new QLabel(tr(""));
    temp->addWidget(picture);

    speed = new QLabel;
    changeItem( 0 );
    temp->addWidget(speed);

    temp->addStretch(1);
    all->addLayout(temp);

    temp = new QVBoxLayout;
    standarts = new QListWidget;
    temp->addWidget(standarts);
    standarts->addItem( tr("ethernet 10/100") );
    standarts->addItem( tr("ethernet 10/100/1000") );

    addEditButton = new QPushButton(tr("Add"));
    addEditButton->setIcon(QIcon(":/im/images/edit_add.png"));
    setWindowTitle(tr("Add netcard"));
    standarts->setCurrentRow(0);

    cancelButton = new QPushButton(tr("Cancel"));
    cancelButton->setIcon(QIcon(":/im/images/not.png"));

    temp->addStretch(1);

    tempL = new QVBoxLayout;
    tempL->addWidget(addEditButton);
    tempL->addWidget(cancelButton);
    temp->addLayout(tempL);
    all->addLayout(temp);
    setLayout(all);
    resize( sizeHint() );
    connect( standarts , SIGNAL(currentRowChanged(int)) , this , SLOT(changeItem(int)));
    connect( addEditButton , SIGNAL(clicked()) , this , SLOT(checkAccept()));
    connect( cancelButton , SIGNAL(clicked()) , this , SLOT(reject()));
}

void interfaceDialog::checkAccept()
{
    res = standarts->currentRow();
    accept();
}

void interfaceDialog::changeItem(int n)
{
    switch(n) {
        case 0:
            picture->setPixmap(QPixmap(":/im/images/k10i100.png"));
            speed->setText(tr("Speed: 10/100 Mbit/s"));
            break;
        case 1:
            picture->setPixmap(QPixmap(":/im/images/k10i100i1000.png"));
            speed->setText(tr("Speed: 10/100/1000 Mbit/s"));
            break;
    }
}


