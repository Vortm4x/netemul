#include "computer.h"
#include "cabledev.h"
#include "interface.h"
#include "computerproperty.h"
#include <QtDebug>


computer::computer(int c)
{
    for ( int i = 0 ; i < c ; i++)
        addInterface(QString("eth%1").arg(i+1),interface::ethernet100);
    addToTable(tr("127.0.0.0"),tr("255.0.0.0"),tr("127.0.0.1"),tr("127.0.0.1"),0,connectMode);
    setNote(trUtf8("Компьютер"));
}

void computer::dialog()
{
    computerProperty *d = new computerProperty;
    d->setComputer(this);
    d->exec();
    delete d;
}



