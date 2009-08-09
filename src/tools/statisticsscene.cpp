#include "statisticsscene.h"
#include "mycanvas.h"
#include "device.h"

statisticsScene::statisticsScene(myCanvas *scene)
{
    myScene = scene;
    comps = 0 ; routers = 0;
    hubs = 0 ; switchs = 0;
    foreach ( device *i , myScene->myDevices ) {
        Statistics += i->deviceStatistics();
        switch ( i->deviceType() ) {
            case device::compDev : comps++; break;
            case device::hubDev : hubs++; break;
            case device::switchDev : switchs++; break;
            case device::routerDev : routers++; break;
            default : qFatal("Fear!!!");
        }
    }
}

QString statisticsScene::devicesString()
{
    QString temp;
    temp += tr("Number of devices: %1\n").arg(  myScene->devicesCount() );
    temp += tr("Number of computers: %1\n").arg( comps );
    temp += tr("Number of hubs: %1\n").arg( hubs );
    temp += tr("Number of switchs: %1\n").arg( switchs );
    temp += tr("Number of routers: %1\n").arg( routers );
    temp += tr("Number of cables: %1\n").arg( myScene->cablesCount() );
    return temp;
}

QString statisticsScene::trafficString()
{
    return Statistics.toString();
}


