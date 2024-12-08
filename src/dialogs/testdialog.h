/****************************************************************************************
** NetEmul - program for simulating computer networks.
** Copyright © 2009 Semenov Pavel and Omilaeva Anastasia
**
** NetEmul is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** NetEmul is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with the NetEmul; if not, write to the Free
** Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
** 02111-1307 USA.
****************************************************************************************/
#ifndef TESTDIALOG_H
#define TESTDIALOG_H

#include <QDialog>
#include <QtScript>
#include "ui_testdialog.h"
//#define __NO_TOOLS__ 0

#ifdef __NO_TOOLS__
#include <QScriptEngineDebugger>
#endif

class MyCanvas;

class testDialog : public QDialog , private Ui::testDialog {
    Q_OBJECT
    Q_DISABLE_COPY(testDialog)
public:
    explicit testDialog(MyCanvas *c,QWidget *parent = 0);
    virtual ~testDialog();
protected:
    virtual void changeEvent(QEvent *e);
private slots:
    void start();
    void selectAll(bool c);
    void selectChange();
    void setScriptPath();
private:
    void updateList();
    QScriptEngine engine;
#ifdef __NO_TOOLS__
    QScriptEngineDebugger debugger;
#endif
    bool test(QString s);
    MyCanvas *canva;
};

#endif // TESTDIALOG_H
