#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>

#include "ui_settingdialog.h"

static const int main_digit = 10000;
static const int little_digit = 20;

class settingDialog : public QDialog , private Ui::settingDialog
{
    Q_OBJECT
    Q_DISABLE_COPY( settingDialog )
public:
    settingDialog();    
public slots:
    void apply();
    void applyDisable();
    void applyEnable();
protected:
    virtual void changeEvent(QEvent *e);
};

#endif // SETTINGDIALOG_H
