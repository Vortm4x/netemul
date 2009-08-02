#include "tableswitch.h"
#include "switchdevice.h"
#include "switchmodel.h"

tableSwitch::tableSwitch(switchTableSetting *s)
{
    sw = s;
    t_sw = s->switchTable();
    setupUi(this);
    table->setModel(t_sw);
    QHeaderView *h = table->horizontalHeader();
    h->setResizeMode( QHeaderView::Stretch );
    int n = s->socketsCount();
    for ( int i = 0 ; i < n ; i++ )
        cb_port->addItem(trUtf8("LAN%1").arg(i+1));
    connect( btn_add , SIGNAL(clicked()) , SLOT(addRecord()) );
    connect( btn_del , SIGNAL(clicked()) , SLOT(deleteRecord()) );
    connect( table->selectionModel() , SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(selectionChanged()) );
    connect( t_sw , SIGNAL(modelReset()) , SLOT(resetSelection()));
}

void tableSwitch::addRecord()
{
    devicePort *t = sw->socket( cb_port->currentText() );
    t_sw->addToTable( macAddress( le_macAddress->text() ) , t  , switchModel::staticMode);
    le_macAddress->setText("00:00:00:00:00:00");
}

void tableSwitch::deleteRecord()
{
    QItemSelectionModel *m = table->selectionModel();
    QModelIndexList l = m->selectedIndexes();
    foreach ( QModelIndex i , l )
        if ( i.column() == 0 ) {
            t_sw->deleteFromTable( i.data(Qt::DisplayRole).toString() );
            return;
        }
}

void tableSwitch::resetSelection()
{
    btn_del->setEnabled(false);
}

void tableSwitch::selectionChanged()
{
    btn_del->setEnabled(true);
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
