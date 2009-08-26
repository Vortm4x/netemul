#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include "ui_aboutwindow.h"

class aboutWindow : public QDialog, private Ui::aboutWindow {
    Q_OBJECT
public:
    aboutWindow(QWidget *parent = 0);

protected:
    void changeEvent(QEvent *e);
};

#endif // ABOUTWINDOW_H
