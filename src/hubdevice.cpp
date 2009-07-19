#include "hubproperty.h"
#include "hubdevice.h"

hubDevice::hubDevice(int c)
{
    chip = new hubChip(c);
    setNote( QObject::trUtf8( "Концентратор" ) );
}

hubDevice::~hubDevice()
{
    delete chip;
}


void hubDevice::read(QDataStream &stream)
{
    int n;
    QString s;
    stream >> n;
    chip->setSocketsCount(n);
    stream >> *chip;
    stream >> s;
    setNote(s);
}

void hubDevice::write(QDataStream &stream) const
{
    stream << hubDev << sockets().count();
    stream << *chip;
    stream << note();
}

void hubDevice::dialog()
{
    hubProperty *d = new hubProperty;
    if ( !d ) return;
    d->setHub(new hubSetting(this));
    d->exec();
    delete d;
}
