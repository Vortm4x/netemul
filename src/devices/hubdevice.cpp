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
    collision = 0;
}

hubDevice::~hubDevice()
{
    delete chip;
}


void hubDevice::detectCollision()
{
    collision++;
    chip->detectCollision();
}

void hubDevice::read(QDataStream &stream)
{
    boxDevice::read(stream);
    stream >> collision;
}

void hubDevice::write(QDataStream &stream) const
{
    stream << hubDev;
    boxDevice::write(stream);
    stream << collision;
}

void hubDevice::dialog()
{
    hubProperty *d = new hubProperty;
    hubSetting *set = new hubSetting(this);
    d->setHub(set);
    d->exec();
    delete set;
}

