#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>

#include "ui_settingdialog.h"

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
