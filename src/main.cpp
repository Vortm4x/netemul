#include <QApplication>
#include <QTime>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Q_INIT_RESOURCE(netemul);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    MainWindow w;
//    if ( argc > 1 ) {
//        char *temp = argv[1];
//        w.setFile( QString( temp ) );
//    }
    w.show();
    return a.exec();
}
