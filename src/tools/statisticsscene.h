#ifndef STATISTICSSCENE_H
#define STATISTICSSCENE_H

#include <QtCore/QString>
#include <QtCore/QObject>
#include "statistics.h"

class myCanvas;

class statisticsScene : public QObject
{
    Q_OBJECT
public:
    statisticsScene(myCanvas *scene);
    QString devicesString();
    QString trafficString();
private:
    myCanvas *myScene;
    statistics Statistics;
    int comps,hubs,switchs,routers;
};

#endif // STATISTICSSCENE_H
