#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include "ui_helpwindow.h"

class helpWindow : public QDialog, private Ui::helpWindow {
    Q_OBJECT
    Q_DISABLE_COPY(helpWindow)
public:
    explicit helpWindow(QWidget *parent = 0);
    void setAddress(QString s);
protected:
    virtual void changeEvent(QEvent *e);
};

#endif // HELPWINDOW_H
