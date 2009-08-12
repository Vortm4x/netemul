#include "boxdevice.h"
#include "logdialog.h"

boxDevice::boxDevice()
{

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

void boxDevice::deciSecondTimerEvent()
{
    chip->deciSecondTimerEvent();
}

void boxDevice::showLogDialog(logDialog *log) const
{
    connect( chip , SIGNAL(receiveData(frame)) , log , SLOT(receiveData(frame)) );
    connect( chip , SIGNAL(sendData(frame)) , log , SLOT(sendData(frame)) );
}



