#include "aboutwindow.h"

aboutWindow::aboutWindow(QWidget *parent) : QDialog(parent)
{
    setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
}

void aboutWindow::changeEvent(QEvent *e)
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
