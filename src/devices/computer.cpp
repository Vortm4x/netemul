#include "computer.h"
#include "cabledev.h"
#include "interface.h"
#include "computerproperty.h"

computer::computer(int c /* = 1 */)
{
    for ( int i = 0 ; i < c ; i++)
        addInterface(trUtf8("eth%1").arg(i));
    addToTable(tr("127.0.0.0"),tr("255.0.0.0"),tr("127.0.0.1"),tr("127.0.0.1"),0,connectMode);
    setNote(trUtf8("Компьютер"));
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



