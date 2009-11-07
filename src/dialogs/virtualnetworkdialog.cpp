#include "virtualnetworkdialog.h"

virtualNetworkDialog::virtualNetworkDialog(QWidget *parent) : QDialog(parent)
{
    setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
}

void virtualNetworkDialog::setDevice( virtualNetworkSetting *device)
{
    myDevice = device;
}

void virtualNetworkDialog::changeEvent(QEvent *e)
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
