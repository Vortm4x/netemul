#include "helpwindow.h"

helpWindow::helpWindow(QWidget *parent) :QDialog(parent)
{
    setupUi(this);
}

void helpWindow::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void helpWindow::setAddress(QString s)
{
    textBrowser->setSource(QUrl(s));
}
