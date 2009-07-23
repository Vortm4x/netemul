#include "switchdevice.h"
#include "switchproperty.h"
#include "switchchip.h"
#include "tableswitch.h"
#include "frame.h"

switchDevice::switchDevice(int c)
{
    chip = new switchChip(c);
    setNote(trUtf8("Коммутатор"));
}

switchDevice::~switchDevice()
{

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
    tableSwitch *d = new tableSwitch;
    d->setSwitch(this);
    d->exec();
    delete d;
}
