#include "tableswitch.h"
#include "switchdevice.h"
#include "switchmodel.h"

tableSwitch::tableSwitch(switchTableSetting *s)
{
    sw = s;
    setupUi(this);
    table->setModel(s->switchTable());
    int n = s->socketsCount();
    for ( int i = 0 ; i < n ; i++ )
        cb_port->addItem(trUtf8("LAN%1").arg(i+1));
    connect( btn_add , SIGNAL(clicked()) , SLOT(addRecord()) );
    connect( btn_del , SIGNAL(clicked()) , SLOT(deleteRecord()) );
}

void tableSwitch::addRecord()
{
    sw->addToTable( macAddress( le_macAddress->text() ) , cb_port->currentText() , switchModel::staticMode , 0);
    le_macAddress->clear();
}
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
    sw->switchTable()->removeRows( table->currentIndex().row() , 1 );
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
