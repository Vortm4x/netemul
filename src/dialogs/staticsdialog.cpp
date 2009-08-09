#include "staticsdialog.h"
#include "mycanvas.h"
#include "statisticsscene.h"

staticsDialog::staticsDialog(statisticsScene *statistics, QWidget *parent) :QDialog(parent)
{
    setupUi(this);
    deviceLabel->setText( statistics->devicesString() );
    trafficLabel->setText( statistics->trafficString() );
    setAttribute(Qt::WA_DeleteOnClose);
    setMinimumSize( sizeHint() );
}

void staticsDialog::changeEvent(QEvent *e)
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
