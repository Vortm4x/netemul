#include "tableswitch.h"
#include "switchdevice.h"

tableSwitch::tableSwitch()
{
    setupUi(this);
    connect( btn_add , SIGNAL(clicked()) , SLOT(addRecord()) );
    connect( btn_del , SIGNAL(clicked()) , SLOT(deleteRecord()) );
}

tableSwitch::~tableSwitch()
{

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
        table->setItem(n-1,0,temp);
        temp = new QTableWidgetItem( i->port->name() );
        table->setItem(n-1,1,temp);
        temp = new QTableWidgetItem( i->modeString() );
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

void tableSwitch::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
