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

    QList<devicePort*> l = start->sockets();
    foreach ( devicePort* i , l)
         new QListWidgetItem( i->connectIcon() , i->name() , startList ,i->isConnect() );

    l = end->sockets();
    foreach ( devicePort* i , l )
        new QListWidgetItem(  i->connectIcon() , i->name() , endList , i->isConnect() );

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
    okButton->setEnabled( !start->socket( startList->currentItem()->text() )->isConnect() &&
                          !end->socket( endList->currentItem()->text() )->isConnect() );
}

QString connectDialog::getStart()
{
    return startList->currentItem()->text() ;
}

QString connectDialog::getEnd()
{
    return  endList->currentItem()->text() ;
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



