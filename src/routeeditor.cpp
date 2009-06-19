#include "routeeditor.h"
#include "ipedit.h"
#include "smartdevice.h"
#include <QPushButton>
#include <QTableWidget>
#include <QSpinBox>
#include <QBoxLayout>
#include <QLabel>

routeEditor::routeEditor()
{
    QVBoxLayout *all = new QVBoxLayout;
    table = new QTableWidget(1,6);
    table->setSelectionBehavior( QAbstractItemView::SelectRows );
    table->setMinimumSize( QSize(700,200) );
    all->addWidget(table);

    ip_dest = new ipEdit(trUtf8("Адрес назначения: "));
    all->addWidget(ip_dest);
    ip_mask = new ipEdit(trUtf8("Маска: "));
    all->addWidget(ip_mask);
    ip_gateway = new ipEdit(trUtf8("Шлюз: "));
    all->addWidget(ip_gateway);
    ip_out = new ipEdit(trUtf8("Интерфейс: "));
    all->addWidget(ip_out);
    QHBoxLayout *lay = new QHBoxLayout;
    QLabel *lb = new QLabel(trUtf8("Метрика: "));
    lay->addWidget(lb);
    sp_metr = new QSpinBox;
    sp_metr->setMinimum(0);
    sp_metr->setMaximum(100);
    lay->addWidget(sp_metr);
    lay->addStretch(1);
    all->addLayout(lay);

    lay = new QHBoxLayout;
    btn_add = new QPushButton( QIcon(":/im/images/edit_add.png"), trUtf8("Добавить"));
    btn_add->setFixedWidth(150);
    connect( btn_add , SIGNAL(clicked()) , SLOT(addRecord()) );
    lay->addWidget( btn_add, 1, Qt::AlignLeft);
    btn_remove = new QPushButton(QIcon(":/im/images/edit_remove.png"),trUtf8("Удалить"));
    btn_remove->setFixedWidth(150);
    connect( btn_remove , SIGNAL(clicked()) ,SLOT(deleteRecord()) );
    lay->addWidget(btn_remove, 1, Qt::AlignLeft);
    lay->addStretch(2);
    btn_close = new QPushButton(QIcon(":/im/images/minus2.png"), trUtf8("Закрыть"));
    btn_close->setFixedWidth(150);
    lay->addWidget(btn_close, 1, Qt::AlignRight);
    all->addLayout(lay);

    connect(btn_close, SIGNAL(clicked()), SLOT(reject()));

    setLayout(all);
}

void routeEditor::setDevice(smartDevice *s)
{
    dev = s;
    updateTable();
}

void routeEditor::updateTable()
{
    int n = 1;
    table->clear();
    QStringList head;
    head << trUtf8("Адрес назначения") << trUtf8("Маска") << trUtf8("Шлюз")
            << trUtf8("Интерфейс") << trUtf8("Метрика") << trUtf8("Источник");
    table->setHorizontalHeaderLabels(head);
    QList<routeRecord*> l = dev->routeTable();
    table->setRowCount(l.count());
    foreach ( routeRecord *i , l ) {
        QTableWidgetItem *temp = new QTableWidgetItem( i->dest.ipString() );
        temp->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        table->setItem(n-1,0,temp);
        temp = new QTableWidgetItem( i->mask.ipString() );
        temp->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        table->setItem(n-1,1,temp);
        temp = new QTableWidgetItem( i->gateway.ipString() );
        temp->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        table->setItem(n-1,2,temp);
        if ( !i->out ) temp = new QTableWidgetItem(tr("127.0.0.1"));
        else temp = new QTableWidgetItem( i->out->ip().ipString() );
        temp->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        table->setItem(n-1,3,temp);
        temp = new QTableWidgetItem( trUtf8("%1").arg(i->metric) );
        temp->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        table->setItem(n-1,4,temp);
        temp = new QTableWidgetItem( trUtf8("%1").arg(i->modeString()) );
        temp->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        table->setItem(n-1,5,temp);
        n++;
    }
    correctSize();
}

void routeEditor::resizeEvent(QResizeEvent *e)
{
    Q_UNUSED(e);
    correctSize();
}

void routeEditor::addRecord()
{
    dev->addToTable( ip_dest->ipText() , ip_mask->ipText() , ip_gateway->ipText() , ip_out->ipText() ,0 , sp_metr->value() );
    ip_dest->clear();
    ip_mask->clear();
    ip_gateway->clear();
    ip_out->clear();
    sp_metr->setValue(0);
    updateTable();
}

void routeEditor::deleteRecord()
{
    if ( table->selectedItems().isEmpty() ) return;
    dev->deleteFromTable( table->currentRow() );
    updateTable();
}

void routeEditor::correctSize()
{
    int n = table->width();
    for ( int i = 0 ; i < 6 ; i++ )
        table->setColumnWidth(i,2*n/11);
    table->setColumnWidth(4,n/11);
}
