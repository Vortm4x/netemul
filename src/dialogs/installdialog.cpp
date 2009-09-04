#include <QtGui/QMessageBox>
#include "installdialog.h"
#include "ripprogramm.h"
#include "smartdevice.h"

//#define DHCP_WORK

installDialog::installDialog(QWidget *parent) : QDialog(parent)
{
    setupUi(this);
#ifdef DHCP_WORK
    list->item(1)->setFlags( list->item(1)->flags() | Qt::ItemIsEnabled );
    list->item(2)->setFlags( list->item(2)->flags() | Qt::ItemIsEnabled );
#endif
    setAttribute(Qt::WA_DeleteOnClose);
}

void installDialog::setDevice(smartDevice *d)
{
    smart = d;
}
/*!
  Слот устанавливает выбранную программу,
  закрывает диалог.
*/
void installDialog::install()
{
    if ( smart->hasProgramm( list->currentItem()->text() ) ) {
            QMessageBox::warning(0,tr("Error") , tr("Program already installed.") ,
                                 QMessageBox::Ok , QMessageBox::Ok );
            return;
    }
    if ( list->currentRow() < 0 ) return;
    programm a(list->currentRow()+50);
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
