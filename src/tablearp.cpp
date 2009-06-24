#include "tablearp.h"
#include "ipedit.h"
#include "smartdevice.h"
#include <QPushButton>
#include <QTableWidget>
#include <QLineEdit>
#include <QBoxLayout>
#include <QLabel>
#include <QComboBox>

tableArp::tableArp(QWidget *parent) : QDialog(parent)
{
    QVBoxLayout *all = new QVBoxLayout;
    table = new QTableWidget(0,4);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setMinimumWidth(300);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QStringList head;
    head << trUtf8("Mac-адрес") << trUtf8("Ip-адрес") << trUtf8("Тип записи") << trUtf8("Имя интерфейса");
    table->setHorizontalHeaderLabels(head);    
    all->addWidget(table);
    QHBoxLayout *temp = new QHBoxLayout;
    QLabel *lb = new QLabel(trUtf8("Mac-адрес: "));
    temp->addWidget(lb);
    le_mac = new QLineEdit;
    le_mac->setFixedWidth(150);
    le_mac->setInputMask("HH:HH:HH:HH:HH:HH;_");
    le_mac->setText("00:00:00:00:00:00");
    temp->addWidget(le_mac);
    ip = new ipEdit(trUtf8("Ip-адрес: "));
    temp->addWidget(ip);
    lb = new QLabel( trUtf8("Интерфейс") );
    cb_port = new QComboBox;
    cb_port->setFixedWidth(100);    
    temp->addWidget(lb);
    temp->addWidget(cb_port);
    all->addLayout(temp);
    temp = new QHBoxLayout;
    btn_add = new QPushButton( QIcon(":im/images/edit_add.png"), trUtf8( "Добавить" ));
    connect(btn_add, SIGNAL(clicked()), SLOT(addRecord()));
    btn_del = new QPushButton( QIcon( ":im/images/edit_remove.png" ), trUtf8( "Удалить" ));
    connect(btn_del, SIGNAL(clicked()), SLOT(deleteRecord()));
    btn_close = new QPushButton( QIcon( ":im/images/minus2.png" ), trUtf8( "Закрыть" ));
    connect(btn_close, SIGNAL(clicked()), SLOT(reject()));
    temp->addWidget(btn_add);
    temp->addWidget(btn_del);
    temp->addStretch(1);
    temp->addWidget(btn_close);
    all->addLayout(temp);
    setLayout(all);
}

void tableArp::setSmart(smartDevice *dev)
{
    d = dev;
    updateTable();
}

void tableArp::correctSize()
{
    int n = table->width();
    for ( int i = 0; i < 4; i++)
        table->setColumnWidth(i, n/4);
}

void tableArp::updateTable()
{
    table->clearContents();
    cb_port->clear();
    table->setRowCount(0);
    QList<devicePort*> list = d->sockets();
    foreach ( devicePort *p, list ) {
        if ( !p->isConnect() ) continue;
        cb_port->addItem(QIcon(":im/images/ok.png"), p->name() );
        QList<arpRecord*> rec = qobject_cast<interface*>(p->parentDev())->arpTable();
        foreach ( arpRecord *r, rec ){
            table->insertRow(table->rowCount());
            QTableWidgetItem *ti_mac = new QTableWidgetItem(r->mac.macString());
            QTableWidgetItem *ti_ip = new QTableWidgetItem(r->ip.ipString());
            QTableWidgetItem *ti_mode = new QTableWidgetItem(r->modeString());
            QTableWidgetItem *ti_name = new QTableWidgetItem(p->name());
            table->setItem( table->rowCount()-1, 0, ti_mac);
            table->setItem( table->rowCount()-1, 1, ti_ip);
            table->setItem( table->rowCount()-1, 2, ti_mode);
            table->setItem( table->rowCount()-1, 3, ti_name);
        }
    }
    correctSize();
}

void tableArp::addRecord()
{
    if ( le_mac->text() == "00:00:00:00:00:00" || ip->text() == "0.0.0.0" ) return;
    d->adapter( cb_port->currentText() )->addToTable( ip->text(), le_mac->text(), interface::staticMode );
    updateTable();
    ip->setText("0.0.0.0");
    le_mac->setText("00:00:00:00:00:00");
}

void tableArp::deleteRecord()
{    
    if ( table->selectedItems().isEmpty() ) return;
    int n = table->currentRow();
    d->adapter( table->item( n, 3)->text() )->removeFromTable( table->item( n, 1)->text() );
    updateTable();
}
