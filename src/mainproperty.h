#ifndef MAINPROPERTY_H
#define MAINPROPERTY_H

#include "ui_mainproperty.h"

class mainProperty : public QDialog, private Ui::mainProperty {
    Q_OBJECT
public:
    mainProperty(QWidget *parent = 0);

protected:
    void changeEvent(QEvent *e);
};

#endif // MAINPROPERTY_H
