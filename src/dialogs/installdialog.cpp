#include "installdialog.h"
#include "ripprogramm.h"
#include "smartdevice.h"

installDialog::installDialog(QWidget *parent) : QDialog(parent)
{
    setupUi(this);
}

void installDialog::setDevice(smartDevice *d)
{
    smart = d;
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
}
//-----------------------------------------------
/*!
  Слот устанавливает выбранную программу,
  закрывает диалог.
*/
void installDialog::install()
{
    int p;
    switch (list->currentRow() ) {
        case 0:
            p = programm::RIP;
            break;
    }
    programm a(p);
    smart->installProgramm(a);
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
