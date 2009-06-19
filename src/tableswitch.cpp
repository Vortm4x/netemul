#include "tableswitch.h"
#include "switchdevice.h"
#include <QTableWidget>
#include <QBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>

tableSwitch::tableSwitch()
{
    QVBoxLayout *all = new QVBoxLayout;
    table = new QTableWidget(1 , 3 , this);
    table->setSelectionBehavior( QAbstractItemView::SelectRows);
    table->setMinimumWidth(500);
    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *temp = new QHBoxLayout;
    layout->addWidget( table );
    le_macAddress = new QLineEdit;
    le_macAddress->setFixedWidth(200);
    cb_port = new QComboBox;
    cb_port->setFixedWidth(100);
    QLabel *t = new QLabel(trUtf8("Mac-адрес: "));
    temp->addWidget(t);
    temp->addWidget(le_macAddress);
    le_macAddress->setText("00:00:00:00:00:00");
    le_macAddress->setInputMask("HH:HH:HH:HH:HH:HH;_");
    t = new QLabel(trUtf8("Порт: "));
    temp->addWidget(t);
    temp->addWidget(cb_port);
    temp->setAlignment(Qt::AlignLeft);
    layout->addLayout(temp);
    temp = new QHBoxLayout;
    btn_add = new QPushButton(QIcon(":/im/images/edit_add.png"),trUtf8("Добавить"));
    connect( btn_add , SIGNAL(clicked()) , SLOT(addRecord()) );
    btn_add->setFixedWidth(150);
    btn_del = new QPushButton(QIcon(":/im/images/edit_remove.png"),trUtf8("Удалить"));
    connect( btn_del , SIGNAL(clicked()) , SLOT(deleteRecord()) );
    btn_del->setFixedWidth(150);
    btn_cancel = new QPushButton(QIcon(":/im/images/minus2.png"), trUtf8("Закрыть"));
    btn_cancel->setFixedWidth(150);
    connect(btn_cancel , SIGNAL(clicked()), SLOT(reject()));
    temp->addWidget(btn_add,0,Qt::AlignRight);
    temp->addWidget(btn_del,0,Qt::AlignRight);
    temp->addStretch(1);
    temp->addWidget(btn_cancel,0,Qt::AlignLeft);
    layout->addLayout(temp);
    all->addLayout(layout);
    setLayout(all);
}

void tableSwitch::setSwitch(switchDevice *d)
{
    sw = d;
    updateTable();
}

void tableSwitch::addRecord()
{
    sw->addToTable( macAddress( le_macAddress->text() ) , cb_port->currentText() , switchDevice::staticMode , 0);
    updateTable();
}

void tableSwitch::updateTable()
{
    int n = 1;
    table->clear();
    cb_port->clear();
    QStringList head;
    head << trUtf8("MAC-адрес") << trUtf8("Порт") << trUtf8("Тип записи");
    table->setHorizontalHeaderLabels(head);
    QList<macRecord*> l = sw->switchTable();
    table->setRowCount(l.count());
    foreach ( macRecord *i , l ) {
        QTableWidgetItem *temp = new QTableWidgetItem( i->mac.macString() );
        temp->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        table->setItem(n-1,0,temp);
        temp = new QTableWidgetItem( i->port->name() );
        temp->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        table->setItem(n-1,1,temp);
        temp = new QTableWidgetItem( i->modeString() );
        temp->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        table->setItem(n-1,2,temp);
        n++;
    }
    for ( int i = 0 ; i < sw->sockets().count() ; i++ )
        cb_port->addItem(trUtf8("LAN%1").arg(i+1) );
    correctSize();
}

void tableSwitch::correctSize()
{
    int n = table->width();
    table->setColumnWidth(0,n/3);
    table->setColumnWidth(1,n/3);
    table->setColumnWidth(2,n/3);
}

void tableSwitch::deleteRecord()
{
    if ( table->selectedItems().isEmpty() ) return;
    int n = table->currentRow();
    sw->deleteFromTable( macAddress( table->item(n,0)->text() ) );
    updateTable();
}
