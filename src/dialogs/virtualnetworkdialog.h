#ifndef VIRTUALNETWORKDIALOG_H
#define VIRTUALNETWORKDIALOG_H

#include "ui_virtualnetworkdialog.h"

class virtualNetworkDialog : public QDialog, private Ui::virtualNetworkDialog {
    Q_OBJECT
public:
    virtualNetworkDialog(QWidget *parent = 0);

protected:
    void changeEvent(QEvent *e);
};

#endif // VIRTUALNETWORKDIALOG_H
