#include <QApplication>
#include <QTime>
#include "mainwindow.h"

/*!
 * Проба !!!!!!!!!!!
*/
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Q_INIT_RESOURCE(netemul);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    MainWindow w;
    w.show();
    return a.exec();
}
