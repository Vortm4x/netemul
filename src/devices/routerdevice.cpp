#include "routerdevice.h"
#include "routerproperty.h"
#include "routemodel.h"
#include "appsetting.h"
#include <QMessageBox>

routerDevice::routerDevice(int c /* = 0 */)
{
    if ( !c ) c = appSetting::defaultRouterCount();
    for ( int i = 0 ; i <  c ; i++ )
        addInterface(trUtf8("LAN%1").arg(i+1));
    setNote(trUtf8("<b>Маршрутизатор</b><!--Доступен HTML.-->"));
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
            QMessageBox::warning(0,trUtf8("Error"), trUtf8("Для изменения количества портов отсоедините все провода.")
                                 , QMessageBox::Ok , QMessageBox::Ok);
            return;
        }
    qDeleteAll(myInterfaces);
    myInterfaces.clear();
    for ( int i = 0 ; i < n ; i++ )
        addInterface( trUtf8("LAN%1").arg(i+1) );
}


