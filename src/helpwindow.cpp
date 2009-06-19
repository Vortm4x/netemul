#include "helpwindow.h"

helpWindow::helpWindow(QWidget *parent) :
    QWidget(parent){
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
