#include "tablearp.h"
#include "ipedit.h"
#include "arpmodel.h"
#include "smartdevice.h"
#include <QPushButton>
#include <QTableWidget>
#include <QLineEdit>
#include <QBoxLayout>
#include <QLabel>
#include <QComboBox>

/**
   Конструктор создает основной интерфейс окна.
*/
tableArp::tableArp(QWidget *parent) : QDialog(parent)
{
    QVBoxLayout *all = new QVBoxLayout;
    table = new QTableWidget(0,5);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setMinimumWidth(300);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QStringList head;
    head << trUtf8("Mac-адрес") << trUtf8("Ip-адрес") << trUtf8("Тип записи") << trUtf8("Имя интерфейса")
        << trUtf8("Время жизни") ;
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
//------------------------------------------------------------------
/**
  Назначает диалогу устройство класса smartDevice, именно с этим устройством,
  диалог и будет работать.
*/
void tableArp::setDevice(smartDevice *dev)
{
    d = dev;
    updateTable();
}
//------------------------------------------------------------------
/**
  Корректирует размер колонок таблицы в соответствии с размером окна.
*/
void tableArp::correctSize()
{
    int n = table->width();
    for ( int i = 0; i < 5; i++)
        table->setColumnWidth(i, n/5);
}
//-------------------------------------------------------------------------
/**
  Обновляет содержимое таблицы арп записей при каких либо изменениях.
*/
void tableArp::updateTable()
{
//    table->clearContents();
//    cb_port->clear();
//    table->setRowCount(0);
//    QStringList s = d->sockets();
//    QList<arpModel*> list = d->arpModels();
//    int i = 0;
//    foreach ( arpModel *p, list ) {
//        if ( !d->isConnectSocket(s.at(i)) ) continue;
//        cb_port->addItem(QIcon(":im/images/ok.png"), s.at(i) );
//        QList<arpRecord*> rec = p->;
//        foreach ( arpRecord *r, rec ){
//            table->insertRow(table->rowCount());
//            QTableWidgetItem *ti_mac = new QTableWidgetItem(r->mac.macString());
//            QTableWidgetItem *ti_ip = new QTableWidgetItem(r->ip.ipString());
//            QTableWidgetItem *ti_mode = new QTableWidgetItem(r->modeString());
//            QTableWidgetItem *ti_name = new QTableWidgetItem(p->name());
//            QTableWidgetItem *ti_time = new QTableWidgetItem( tr("%1").arg(r->time) );
//            table->setItem( table->rowCount()-1, 0, ti_mac);
//            table->setItem( table->rowCount()-1, 1, ti_ip);
//            table->setItem( table->rowCount()-1, 2, ti_mode);
//            table->setItem( table->rowCount()-1, 3, ti_name);
//            table->setItem( table->rowCount()-1, 4, ti_time);
//        }
//    }
//    correctSize();
}
//---------------------------------------------------------------
/**
  Добавляет в таблицу arp указанного адаптера статическую запись.
  Если значение mac или ip пустые добавления не происходит, обновляет таблицу и
  обнуляет поля для ввода.
*/
void tableArp::addRecord()
{
    if ( le_mac->text() == "00:00:00:00:00:00" || ip->text() == "0.0.0.0" ) return;
//    d->adapter( cb_port->currentText() )->addToTable( ip->text(), le_mac->text(), interface::staticMode );
    updateTable();
    ip->setText("0.0.0.0");
    le_mac->setText("00:00:00:00:00:00");
}
//----------------------------------------------------------------------
/**
  Удалеяет из таблицы существующую запись и обновляет таблицу.
*/
void tableArp::deleteRecord()
{    
    if ( table->selectedItems().isEmpty() ) return;
//    int n = table->currentRow();
//    d->adapter( table->item( n, 3)->text() )->removeFromTable( table->item( n, 1)->text() );
    updateTable();
}
//------------------------------------------------------------------------
