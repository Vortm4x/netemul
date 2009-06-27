#ifndef TESTDIALOG_H
#define TESTDIALOG_H

#include <QtGui/QDialog>
#include "ui_testdialog.h"
#include <QtScript>

class myCanvas;

namespace Ui {
    class testDialog;
}

class testDialog : public QDialog , private Ui::testDialog {
    Q_OBJECT
    Q_DISABLE_COPY(testDialog)
public:
    explicit testDialog(myCanvas *c,QWidget *parent = 0);
    virtual ~testDialog();
public slots:
    void start();
protected:
    virtual void changeEvent(QEvent *e);
private:
    QScriptEngine engine;
    bool test1();
    bool test2();
    bool test4();
    bool test5();
    bool test6();
    bool test7();
    bool test8();
    myCanvas *canva;
};

#endif // TESTDIALOG_H
