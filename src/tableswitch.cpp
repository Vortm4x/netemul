#include "tableswitch.h"
#include "switchmodel.h"

tableSwitch::tableSwitch(switchModel *s)
{
    sw = s;
    setupUi(this);
    cb_port->clear();
    table->setModel(s);
    connect( btn_add , SIGNAL(clicked()) , SLOT(addRecord()) );
    connect( btn_del , SIGNAL(clicked()) , SLOT(deleteRecord()) );
}

tableSwitch::~tableSwitch()
{

}
void tableSwitch::addRecord()
{
    sw->addToTable( macAddress( le_macAddress->text() ) , 0 , switchModel::staticMode , 0);
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
    //if ( table->selectedItems().isEmpty() ) return;
    //int n = table->currentRow();
    //sw->deleteFromTable( macAddress( table->item(n,0)->text() ) );
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
