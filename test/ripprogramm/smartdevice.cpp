#include "smartdevice.h"
#include "routemodel.h"

smartDevice::smartDevice()
{
    table = new routeModel;
    for ( int i = 0 ; i < 3 ; i++ ) {
        interface *t = new interface;
        t->setIp(QObject::tr("192.168.%1.1").arg(i+1) );
        t->setMask(QObject::tr("255.255.255.0"));
        myInterfaces.push_back(t);
    }
}

smartDevice::~smartDevice()
{
    delete table;
}
