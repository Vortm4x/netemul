#include "routerdevice.h"
#include "routerproperty.h"

routerDevice::routerDevice(int c)
{
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


