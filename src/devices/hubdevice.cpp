#include "hubproperty.h"
#include "hubdevice.h"
#include "appsetting.h"
#include "hubchip.h"

hubDevice::hubDevice(int c /* = 0 */ )
{
    if (!c) c = appSetting::defaultHubCount();
    myManual = appSetting::defaultHubManual();
    chip = new hubChip(c);
    setNote(tr( "<b>Hub</b><!--You can use HTML.-->" ) );
}

hubDevice::~hubDevice()
{
    delete chip;
}


void hubDevice::read(QDataStream &stream)
{
    boxDevice::read(stream);
}

void hubDevice::write(QDataStream &stream) const
{
    stream << hubDev;
    boxDevice::write(stream);
}

void hubDevice::dialog()
{
    hubProperty *d = new hubProperty;
    boxSetting *set = new boxSetting(this);
    d->setHub(set);
    d->exec();
    delete d;
    delete set;
}

bool hubDevice::isSharedBusy(cableDev *c) const
{
    return chip->isSharedBusy(c);
}
