#include "tableswitch.h"
#include "switchdevice.h"

tableSwitch::tableSwitch()
{
    setupUi(this);
    QStringList head;
    head << trUtf8("MAC-адрес") << trUtf8("Порт") << trUtf8("Тип записи") << trUtf8("Время жизни");
    table->setHorizontalHeaderLabels(head);
    cb_port->clear();
    connect( btn_add , SIGNAL(clicked()) , SLOT(addRecord()) );
    connect( btn_del , SIGNAL(clicked()) , SLOT(deleteRecord()) );
}

tableSwitch::~tableSwitch()
{

}

void tableSwitch::setSwitch(switchDevice *d)
{
    sw = d;
    for ( int i = 0 ; i < sw->sockets().count() ; i++ )
        cb_port->addItem(trUtf8("LAN%1").arg(i+1) );
    updateTable();
}

void tableSwitch::addRecord()
{
    Q_ASSERT( false );
    //sw->addToTable( macAddress( le_macAddress->text() ) , cb_port->currentText() , switchDevice::staticMode , 0);
    updateTable();
}
/*!
  Обновляет содержимое таблицы после внесения в неё изменений.
*/
void tableSwitch::updateTable()
{
//    int n = 1;
//    table->clearContents(); // Очищаем таблицу
//    QList<macRecord*> l = sw->switchTable();
//    table->setRowCount(l.count());
//    foreach ( macRecord *i , l ) { // Перебираем все записи у свитча
//        QTableWidgetItem *temp = new QTableWidgetItem( i->mac.macString() );
//        table->setItem(n-1,0,temp);
//        temp = new QTableWidgetItem( i->port->name() );
//        table->setItem(n-1,1,temp);
//        temp = new QTableWidgetItem( i->modeString() );
//        table->setItem(n-1,2,temp);
//        temp = new QTableWidgetItem( QString::number(i->time) );
//        table->setItem(n-1,3,temp);
//        n++;
//    }
//    correctSize(); // Корректируем размер
}
//---------------------------------------------------------------------
/*!
  Корректирует размер столбцов в соответствии с размером таблицы.
*/
void tableSwitch::correctSize()
{
    int n = table->width();
    for ( int i = 0 ; i < 4 ; i++)
        table->setColumnWidth(i,n/4);

}
//-----------------------------------------------------------------
void tableSwitch::deleteRecord()
{
    if ( table->selectedItems().isEmpty() ) return;
    //int n = table->currentRow();
    //sw->deleteFromTable( macAddress( table->item(n,0)->text() ) );
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
