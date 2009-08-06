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
    standarts->addItem( trUtf8("ethernet 10/100") );
    standarts->addItem( trUtf8("ethernet 10/100/1000") );

    addEditButton = new QPushButton(trUtf8("Добавить"));
    addEditButton->setIcon(QIcon(":/im/images/edit_add.png"));
    setWindowTitle(trUtf8("Добавить адаптер"));
    standarts->setCurrentRow(0);

    cancelButton = new QPushButton(trUtf8("Отмена"));
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
            speed->setText(trUtf8("Скорость: 10/100 Мбит/с"));
            break;
        case 1:
            picture->setPixmap(QPixmap(":/im/images/k10i100i1000.png"));
            speed->setText(trUtf8("Скорость: 10/100/1000 Мбит/с"));
            break;
    }
}


