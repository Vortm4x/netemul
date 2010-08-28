#include "classfactory.h"
#include "device.h"
#include "computer.h"
#include "switchdevice.h"
#include "routerdevice.h"
#include "hubdevice.h"

#include <QtDebug>

ClassFactory::ClassFactory()
{
}

QObject* ClassFactory::createInstance(const QString &str, QObject *parent)
{    
    if ( str == "Device" ) {
        return new Device(parent);
    } else if ( str == "Computer" ) {
        return new Computer(parent);
    } else if ( str == "RouterDevice" ) {
        return new RouterDevice(parent);
    } else if ( str == "SwitchDevice" ) {
        return new SwitchDevice(parent);
    } else if ( str == "HubDevice" ) {
        return new HubDevice(parent);
    } else if ( str == "Interface" ) {
        return new Interface(parent);
    } else {
        return 0;
    }
}
