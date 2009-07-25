#include "connectdialog.h"
#include "device.h"
#include "interface.h"

connectDialog::connectDialog(device *s, device *e)
{
    setupUi(this);
    start = s;
    end = e;
    setWindowTitle(trUtf8("Укажите начальные настройки соединения"));
    connect( startList , SIGNAL(itemSelectionChanged()) , SLOT(changeSelect()));
    connect( endList , SIGNAL(itemSelectionChanged()) , SLOT(changeSelect()));

    foreach ( QString i , start->sockets() )
         new QListWidgetItem( connectIcon( start->isConnectSocket(i) ) , i , startList, start->isConnectSocket(i) );

    foreach ( QString i , end->sockets() )
        new QListWidgetItem( connectIcon( end->isConnectSocket(i) ) , i , endList, end->isConnectSocket(i) );

    for ( int i = 0 ; i < startList->count() ; i++ )
        if ( !startList->item(i)->type() ) {
            startList->setCurrentRow(i);
            break;
        }

    for ( int i = 0 ; i < endList->count() ; i++ )
        if ( !endList->item(i)->type() ) {
            endList->setCurrentRow(i);
            break;
        }
    resize( sizeHint());
}

void connectDialog::changeSelect()
{
    if ( startList->currentRow() < 0 || endList->currentRow() < 0 ) {
        okButton->setEnabled(false);
        return;
    }
    okButton->setEnabled( !start->isConnectSocket( startList->currentItem()->text() ) &&
                          !end->isConnectSocket( endList->currentItem()->text() ) );
}

void connectDialog::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

QIcon connectDialog::connectIcon(bool b) const
{
    if (b) return QIcon(":/im/images/ok.png");
    else return QIcon(":/im/images/minus2.png");
}



