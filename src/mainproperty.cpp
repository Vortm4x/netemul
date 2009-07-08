#include "mainproperty.h"

mainProperty::mainProperty(QWidget *parent) :
    QDialog(parent){
    setupUi(this);
}

void mainProperty::changeEvent(QEvent *e)
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
