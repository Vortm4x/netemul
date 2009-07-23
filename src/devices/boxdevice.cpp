#include "boxdevice.h"

boxDevice::boxDevice()
{
    myManual = false;
}

boxDevice::~boxDevice()
{
}

void boxDevice::write(QDataStream &stream) const
{
    deviceImpl::write(stream);
    chip->write(stream);
}

void boxDevice::read(QDataStream &stream)
{
    deviceImpl::read(stream);
    chip->read(stream);
}



