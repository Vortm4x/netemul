#include "computer.h"
#include "interface.h"
#include "routemodel.h"
#ifndef __TESTING__
#include "computerproperty.h"
#endif
#include "appsetting.h"

computer::computer(int c /* = 0 */)
{
    if ( !c ) c = appSetting::defaultComputerCount();
    for ( int i = 0 ; i < c ; i++)
        addInterface(trUtf8("eth%1").arg(i));
    myRouteTable->addToTable(tr("127.0.0.0"),tr("255.0.0.0"),tr("127.0.0.1"),tr("127.0.0.1"),0,routeModel::connectMode);
    setNote(trUtf8("Компьютер"));
}

void computer::dialog()
{
#ifndef __TESTING__
    computerSetting *set = new computerSetting(this);
    computerProperty *d = new computerProperty;
    d->setDevice(set);
    d->exec();
    delete d;
    delete set;
#endif
}



