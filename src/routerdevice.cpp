#include "routerdevice.h"
#include "routerproperty.h"

routerDevice::routerDevice(int c)
{
    int i;
    for ( i = 1 ; i <=  c ; i++ ) {
        QString t = trUtf8("LAN%1").arg(i);
        addInterface(t,0);
    }
    setNote(trUtf8("Маршрутизатор"));
}

interface* routerDevice::addInterface(QString str,int t)
{
    devicePort *tempPort = new devicePort;
    interface *tempInter = new interface(tempPort, t);
    tempInter->setSmart(this);
    tempPort->setParentDev(tempInter);
    tempPort->setConnect(false, NULL);
    tempPort->setName(str);
    addSocket(tempPort);
    return tempPort;
}

void routerDevice::receiveEvent(frame *fr , devicePort *sender)
{
    Q_UNUSED(sender);
    delete fr;
}

void routerDevice::dialog()
{
    routerProperty *d = new routerProperty;
    d->setRouter(this);
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


