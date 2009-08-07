#include "switchdevice.h"
#include "tableswitch.h"
#include "switchproperty.h"
#include "switchchip.h"
#include "appsetting.h"

switchDevice::switchDevice(int c /* =0 */)
{
    if (!c) c = appSetting::defaultSwitchCount();
    myManual = appSetting::defaultSwitchManual();
    chip = new switchChip(c);
    setNote(trUtf8("<b>Switch</b><!--You can use HTML.-->"));
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
    delete t;
    delete set;
}

switchChip* switchDevice::concreteChip()
{
    return static_cast<switchChip*>(chip);
}

void switchDevice::secondTimerEvent()
{
    chip->secondTimerEvent();
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
switchModel* switchTableSetting::switchTable()
{
    return sw->concreteChip()->model();
}


