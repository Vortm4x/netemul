#include "computer.h"
#include "interface.h"
#include "routemodel.h"
#include "computerproperty.h"
#include "appsetting.h"

computer::computer(int c /* = 0 */)
{
    if ( !c ) c = appSetting::defaultComputerCount();
    for ( int i = 0 ; i < c ; i++)
        addInterface(trUtf8("eth%1").arg(i));
    myRouteTable->addToTable("127.0.0.0","255.0.0.0","127.0.0.1","127.0.0.1",0,routeModel::connectMode);
    setNote(trUtf8("<b>Computer</b><!--You can use HTML.-->"));
}

void computer::dialog()
{
    computerSetting *set = new computerSetting(this);
    computerProperty *d = new computerProperty;
    d->setDevice(set);
    d->exec();
    delete d;
    delete set;
}



