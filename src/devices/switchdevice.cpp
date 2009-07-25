#include "switchdevice.h"
#include "tableswitch.h"
#include "switchproperty.h"
#include "switchchip.h"
#include "frame.h"

switchDevice::switchDevice(int c)
{
    chip = new switchChip(c);
    setNote(trUtf8("Коммутатор"));
}

switchDevice::~switchDevice()
{
    delete chip;
}

void switchDevice::write(QDataStream &stream) const
{
    stream << switchDev;
    boxDevice::write(stream);
}

void switchDevice::read(QDataStream &stream)
{
    boxDevice::read(stream);
}

void switchDevice::dialog()
{
    switchProperty *d = new switchProperty;
    boxSetting *set = new boxSetting(this);
    d->setSwitch(set);
    d->exec();
    delete set;
    delete d;
}

void switchDevice::tableDialog()
{
    switchTableSetting *set = new switchTableSetting(this);
    tableSwitch *t = new tableSwitch(set);
    t->exec();
    delete set;
    delete t;
}

switchChip* switchDevice::concreteChip()
{
    return static_cast<switchChip*>(chip);
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

void switchTableSetting::addToTable(const macAddress &m , const QString &p, int mode , int time)
{
    sw->concreteChip()->addToSwitchTable(m,p,mode,time);
}

switchModel* switchTableSetting::switchTable()
{
    return sw->concreteChip()->model();
}
