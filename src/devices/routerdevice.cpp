#include "routerdevice.h"
#include "routerproperty.h"
#include "routemodel.h"
#include "appsetting.h"
#include <QMessageBox>

routerDevice::routerDevice(int c /* = 0 */)
{
    if ( !c ) c = appSetting::defaultRouterCount();
    for ( int i = 0 ; i <  c ; i++ )
        addInterface(tr("LAN%1").arg(i+1));
    setNote(tr("<b>Router</b><!--You can use HTML.-->"));
}

void routerDevice::dialog()
{
    routerProperty *d = new routerProperty;
    routerSetting *set = new routerSetting(this);
    d->setRouter(set);
    d->exec();
    delete d;
    delete set;
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
    foreach ( interface *i , myInterfaces )
        if ( i->isConnect() ) {
            QMessageBox::warning(0,tr("Error"), tr("To change the number of ports, disconnect all cables!"),
                                 QMessageBox::Ok , QMessageBox::Ok);
            return;
        }
    qDeleteAll(myInterfaces);
    myInterfaces.clear();
    for ( int i = 0 ; i < n ; i++ )
        addInterface( tr("LAN%1").arg(i+1) );
}


