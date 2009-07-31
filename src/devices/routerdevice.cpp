#include "routerdevice.h"
#include "routerproperty.h"
#include "routemodel.h"
#include "appsetting.h"

routerDevice::routerDevice(int c /* = 0 */)
{
    if ( !c ) c = appSetting::defaultRouterCount();
    for ( int i = 0 ; i <  c ; i++ )
        addInterface(trUtf8("LAN%1").arg(i+1));
    setNote(trUtf8("Маршрутизатор"));
}

void routerDevice::dialog()
{
    routerProperty *d = new routerProperty;
    smartSetting *set = new smartSetting(this);
    d->setRouter(set);
    d->exec();
    delete d;
}

void routerDevice::write(QDataStream &stream) const
{
    stream << routerDev;
    smartDevice::write(stream);
}

void routerDevice::read(QDataStream &stream)
{
   smartDevice::read(stream);
}

void routerDevice::setSocketsCount(int n)
{
    qDeleteAll(myInterfaces);
    myInterfaces.clear();
    for ( int i = 0 ; i < n ; i++ )
        addInterface( trUtf8("LAN%1").arg(i+1) );
}


