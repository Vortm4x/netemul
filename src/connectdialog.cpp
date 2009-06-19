#include "connectdialog.h"
#include "device.h"
#include "interface.h"
#include "computer.h"
#include "switchdevice.h"
#include "hubdevice.h"
#include <QPushButton>
#include <QBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QApplication>

connectDialog::connectDialog(device *start, device *end)
{
    bool s = false;
    QListWidgetItem *c;
    setWindowTitle(trUtf8("Укажите начальные настройки соединения"));
    QHBoxLayout *tempLayout = new QHBoxLayout;
    QVBoxLayout *allLayout = new QVBoxLayout;
    okButton = new QPushButton(QIcon(":/im/images/ok.png"),trUtf8("Создать"),this);
    okButton->setEnabled(false);
    cancelButton = new QPushButton(QIcon(":/im/images/minus2.png"),trUtf8("Отмена"),this);

    startList = new QListWidget;
    connect( startList , SIGNAL(itemSelectionChanged()) , this , SLOT(changeSelect()));
    endList = new QListWidget;
    connect( endList , SIGNAL(itemSelectionChanged()) , this , SLOT(changeSelect()));

    QList<devicePort*> inters = start->sockets();
    foreach ( devicePort* item , inters ) {
        QListWidgetItem *temp = new QListWidgetItem( item->name() );
        temp->setIcon(item->connectIcon());
        if ( !s && !item->isConnect() ) { c = temp; s = true; }
        startList->addItem(temp);
        startInterfaceList << item;
    }
    if (s) startList->setCurrentItem(c);
    s = false;
    inters = end->sockets();
    foreach ( devicePort* item , inters ) {
        QListWidgetItem *temp = new QListWidgetItem( item->name() );
        temp->setIcon(item->connectIcon());
        if ( !s && !item->isConnect() ) { c = temp; s = true; }
        endList->addItem(temp);
        endInterfaceList << item;
    }
    if(s) endList->setCurrentItem(c);
    tempLayout->addWidget(startList);
    tempLayout->addWidget(endList);
    allLayout->addWidget( new QLabel(trUtf8("Выберите соединяемые интерфейсы:")) , 0 , Qt::AlignCenter);
    allLayout->addLayout(tempLayout);
    tempLayout = new QHBoxLayout;
    tempLayout->addWidget(okButton, 1 , Qt::AlignCenter);
    tempLayout->addWidget(cancelButton, 1 , Qt::AlignCenter);
    allLayout->addLayout(tempLayout);
    setLayout(allLayout);
    connect( okButton , SIGNAL(pressed()), this , SLOT(accept()));
    connect( cancelButton , SIGNAL(pressed()) , this , SLOT(reject()));
    resize( sizeHint());
}

void connectDialog::changeSelect()
{
    if ( startList->currentRow() < 0 || endList->currentRow() < 0 ) {
        okButton->setEnabled(false);
        return;
    }
    okButton->setEnabled( devicePort::isCompability( startInterfaceList.at( startList->currentRow() ),
                                                     endInterfaceList.at(endList->currentRow()) ) ) ;
}

QString connectDialog::getStart()
{
    return startInterfaceList.at( startList->currentRow() )->name() ;
}

QString connectDialog::getEnd()
{
    return endInterfaceList.at( endList->currentRow() )->name() ;
}



