#ifndef VIRTUALNETWORKDIALOG_H
#define VIRTUALNETWORKDIALOG_H

#include "ui_virtualnetworkdialog.h"

class VirtualNetworkSetting;

class virtualNetworkDialog : public QDialog, private Ui::virtualNetworkDialog {
    Q_OBJECT
public:
    virtualNetworkDialog(QWidget *parent = 0);
    void setDevice( VirtualNetworkSetting *device);
protected:
    void changeEvent(QEvent *e);
private:
    VirtualNetworkSetting *myDevice;
};

#endif // VIRTUALNETWORKDIALOG_H
