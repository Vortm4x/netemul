#ifndef RIPPROPERTY_H
#define RIPPROPERTY_H

#include "ui_ripproperty.h"

class ripProgramm;

class ripProperty : public QDialog, private Ui::ripProperty {
    Q_OBJECT
public:
    ripProperty(QWidget *parent = 0);
    void setProgramm( ripProgramm *prog);
protected:
    void changeEvent(QEvent *e);
private slots:
    void apply();
private:
    ripProgramm *myProgramm;
};

#endif // RIPPROPERTY_H
