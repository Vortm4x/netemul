#ifndef VIRTUALNETWORKDIALOG_H
#define VIRTUALNETWORKDIALOG_H

#include "ui_virtualnetworkdialog.h"

class virtualNetworkSetting;

class virtualNetworkDialog : public QDialog, private Ui::virtualNetworkDialog {
    Q_OBJECT
public:
    virtualNetworkDialog(QWidget *parent = 0);
    void setDevice( virtualNetworkSetting *device);
protected:
    void changeEvent(QEvent *e);
private:
    virtualNetworkSetting *myDevice;
};

#endif // VIRTUALNETWORKDIALOG_H
