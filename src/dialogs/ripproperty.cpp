#include "ripproperty.h"
#include "ripprogramm.h"

ripProperty::ripProperty(QWidget *parent) : QDialog(parent)
{
    setupUi(this);
    setAttribute( Qt::WA_DeleteOnClose );
}

void ripProperty::setProgramm( ripProgramm *prog)
{
    myProgramm = prog;
    cb_split->setCurrentIndex( myProgramm->splitMode() );
    chb_update->setChecked( myProgramm->hasTriggerUpdate() );
}

void ripProperty::apply()
{
    myProgramm->setSplitMode( cb_split->currentIndex() );
    myProgramm->setHasTriggerUpdate( chb_update->isChecked() );
    accept();
}

void ripProperty::changeEvent(QEvent *e)
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
