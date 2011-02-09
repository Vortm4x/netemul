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
#include "statisticsscene.h"
#include "mycanvas.h"
#include "device.h"

StatisticsScene::StatisticsScene(MyCanvas *scene)
{
    myScene = scene;
    comps = 0 ; routers = 0;
    hubs = 0 ; switchs = 0;
    foreach ( Device *i , myScene->myDevices ) {
        statistics += i->deviceStatistics();
        switch ( i->deviceType() ) {
            case Device::compDev : comps++; break;
            case Device::hubDev : hubs++; break;
            case Device::switchDev : switchs++; break;
            case Device::routerDev : routers++; break;
            default : qFatal("Fear!!!");
        }
    }
}

QString StatisticsScene::devicesString()
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

QString StatisticsScene::trafficString()
{
    return statistics.toString();
}


