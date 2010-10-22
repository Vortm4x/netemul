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
#include <QtCore/QtDebug>
#include <QtCore/QFile>
#include <QtGui/QFileDialog>
#include "testdialog.h"
#include "mycanvas.h"
#include "deviceimpl.h"
#include "appsetting.h"

QScriptValue ObjectToScript(QScriptEngine *engine, DeviceImpl* const &in)
{
    return engine->newQObject(in);
}

void ObjectFromScript(const QScriptValue &object, DeviceImpl* &out)
{
    out = qobject_cast<DeviceImpl*>(object.toQObject());
}

QScriptValue ObjectToScriptItem(QScriptEngine *engine, TextItem* const &in)
{
    return engine->newQObject(in);
}

void ObjectFromScriptItem(const QScriptValue &object, TextItem* &out)
{
    out = qobject_cast<TextItem*>(object.toQObject());
}


testDialog::testDialog(MyCanvas *c,QWidget *parent) : QDialog(parent)
{
    setupUi(this);
    canva = c;   
    updateList();
    QScriptValue p = engine.newQObject(c);
#ifdef __NO_TOOLS__
    debugger.attachTo(&engine);
    debugger.action(QScriptEngineDebugger::InterruptAction)->trigger();
#endif
    engine.globalObject().setProperty("scene",p);
    qScriptRegisterMetaType(&engine, ObjectToScript, ObjectFromScript);
    qScriptRegisterMetaType(&engine, ObjectToScriptItem, ObjectFromScriptItem);
    connect( checkBox , SIGNAL(toggled(bool)) , SLOT(selectAll(bool)));
    connect( listWidget , SIGNAL(currentRowChanged(int)) , SLOT(selectChange()));
    setAttribute(Qt::WA_DeleteOnClose);
}

testDialog::~testDialog()
{
}

void testDialog::updateList()
{
    listWidget->clear();
    QDir t(appSetting::scriptPath());
    QStringList h;
    h << "*.js";
    QStringList s = t.entryList(h);
    foreach ( QString i , s)
        listWidget->addItem( i.remove(".js") );
}

void testDialog::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

void testDialog::start()
{
    if ( listWidget->selectedItems().isEmpty() ) return;
    bool res = true;
    startButton->setEnabled(false);
    QList<QListWidgetItem*> l = listWidget->selectedItems();
    foreach ( QListWidgetItem *i , l )
        if ( !test( i->text() ) ) {
            res = false;
            break;
        }
    if (res ) {
        label->setText(tr("Script finished correctly"));
        label->setStyleSheet("background : green");
    } else {
        label->setText(tr("Script not correctly finished"));
        label->setStyleSheet("background : red");
    }
    startButton->setEnabled(true);
}

bool testDialog::test(QString s)
{
    qDebug() << s << " started";
    QString g = appSetting::scriptPath()+s+".js";
    QFile file(g);
    file.open(QIODevice::ReadOnly);
    QString temporary = file.readAll();
    temporary.prepend("with (scene) {");
    temporary.append(" }");
    QScriptValue t = engine.evaluate(temporary);
    if ( t.isError() ) {
        qDebug() << "ERROR in " << g << " file";
        file.close();
        return false;
    }
    file.close();
    if ( !t.toBool() ) qDebug() << s << " script failed";
    qDebug() << s << " finished";
    return t.toBool();
}

void testDialog::selectAll(bool c)
{
    if ( c )
        listWidget->selectAll();
    else {
        if (listWidget->count() ) listWidget->setCurrentRow(0);
    }
}

void testDialog::setScriptPath()
{
    QString name = QFileDialog::getExistingDirectory( this,
                   tr("Choose a directory with scripts"), appSetting::scriptPath() );
    if ( name.isEmpty() ) return;
    if ( name.at( name.length() - 1 ) != '/' ) name.push_back('/');
    appSetting::setScriptPath(name);
    updateList();
}

void testDialog::selectChange()
{
    label->setText(tr("Click start"));
    label->setStyleSheet("");
}

