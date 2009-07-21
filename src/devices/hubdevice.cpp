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
    QString s;
    stream >> *chip;
    stream >> s;
    setNote(s);
}

void hubDevice::write(QDataStream &stream) const
{
    stream << hubDev;
    stream << *chip;
    stream << note();
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
