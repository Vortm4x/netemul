#include "interfacedialog.h"
#include "interface.h"
#include <QPushButton>
#include <QLabel>
#include <QBoxLayout>
#include <QListWidget>
#include <QLineEdit>

interfaceDialog::interfaceDialog()
{
    QHBoxLayout *all = new QHBoxLayout;
    QVBoxLayout *temp = new QVBoxLayout;
    QVBoxLayout *tempL = new QVBoxLayout;

    picture = new QLabel(trUtf8(""));
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

    addEditButton = new QPushButton(trUtf8("Add"));
    addEditButton->setIcon(QIcon(":/im/images/edit_add.png"));
    setWindowTitle(tr("Add netcard"));
    standarts->setCurrentRow(0);

    cancelButton = new QPushButton(trUtf8("Cancel"));
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
            speed->setText(trUtf8("Speed: 10/100 Mbit/s"));
            break;
        case 1:
            picture->setPixmap(QPixmap(":/im/images/k10i100i1000.png"));
            speed->setText(trUtf8("Speed: 10/100/1000 Mbit/s"));
            break;
    }
}


