#include "testdialog.h"
#include <QtDebug>
#include <QFile>
#include "mycanvas.h"


testDialog::testDialog(myCanvas *c,QWidget *parent) : QDialog(parent)
{
    setupUi(this);
    canva = c;
    QDir t("test");
    QStringList h;
    h << "*.js";
    QStringList s = t.entryList(h);
    foreach ( QString i , s)
        listWidget->addItem( i.remove(".js") );
    QScriptValue p = engine.newQObject(c);
    engine.globalObject().setProperty("canva",p);
    connect( startButton , SIGNAL(clicked()), this , SLOT(start()));
    connect( checkBox , SIGNAL(toggled(bool)) , SLOT(selectAll(bool)));
}

testDialog::~testDialog()
{

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
        label->setText(trUtf8("Script finished correctly"));
        label->setStyleSheet("background : green");
    } else {
        label->setText(trUtf8("Script finished is not correctly"));
        label->setStyleSheet("background : red");
    }
    startButton->setEnabled(true);
}

bool testDialog::test(QString s)
{
    qDebug() << s << " started";
    QString g = "test/"+s+".js";
    QFile file(g);
    file.open(QIODevice::ReadOnly);
    QString temporary = file.readAll();
    temporary.prepend("with (canva) {");
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

