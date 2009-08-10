#ifndef TESTDIALOG_H
#define TESTDIALOG_H

#include <QtGui/QDialog>
#include <QtScript>
#include "ui_testdialog.h"
//#define __NO_TOOLS__ 0

#ifdef __NO_TOOLS__
#include <QScriptEngineDebugger>
#endif

class myCanvas;

class testDialog : public QDialog , private Ui::testDialog {
    Q_OBJECT
    Q_DISABLE_COPY(testDialog)
public:
    explicit testDialog(myCanvas *c,QWidget *parent = 0);
    virtual ~testDialog();
public slots:
    void start();
    void selectAll(bool c);
protected:
    virtual void changeEvent(QEvent *e);
private:
    QScriptEngine engine;
#ifdef __NO_TOOLS__
    QScriptEngineDebugger debugger;
#endif
    bool test(QString s);
    myCanvas *canva;
};

#endif // TESTDIALOG_H
