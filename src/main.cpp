#include <QApplication>
#include <QTime>
#include <QTranslator>
#include <QLocale>
#include <QtDebug>
#include "mainwindow.h"

/*!
 * Проба !!!!!!!!!!!
*/
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    a.setStyleSheet("QMenuBar { background-color: rgb(65, 65, 65); } QMenu { background-color: rgb(65, 65, 65); }");
    Q_INIT_RESOURCE(netemul);
    QTranslator t;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    MainWindow w;
    w.show();
    return a.exec();
}
