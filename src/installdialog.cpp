#include "installdialog.h"
#include "ripprogramm.h"
#include "smartdevice.h"

installDialog::installDialog(QWidget *parent) : QDialog(parent)
{
    setupUi(this);
}

void installDialog::setDevice(smartDevice *d)
{
    s = d;
    updateList();
}
/*!
  Обновляет список программ.
*/
void installDialog::updateList()
{
    list->clear();
    QStringList l;
    l << trUtf8("RIP");
    list->addItems(l);
    for ( int i = 0; i < list->count(); i++ ) {
        QString str = list->item(i)->text();
        if ( s->programmAt(str)  &&  s->programmAt(str)->isEnable() ) list->item(i)->setFlags(Qt::NoItemFlags);
    }
}
//-----------------------------------------------
/*!
  Слот устанавливает выбранную программу,
  закрывает диалог.
*/
void installDialog::install()
{
    programm *p;
    switch (list->currentRow() ) {
        case 0:
            p = new ripProgramm( s );
    }
    s->installProgramm(p);
    accept();
}
//-----------------------------------------------
void installDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
