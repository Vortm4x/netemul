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
    connect( log , SIGNAL(changeInterface(QString)) , this , SLOT(setCheckedSocket(QString)) );
    connect( chip , SIGNAL(receiveData(frame,QString)) , log , SLOT(receiveData(frame,QString)) );
    connect( chip , SIGNAL(sendData(frame,QString)) , log , SLOT(sendData(frame,QString)) );
}

void boxDevice::setCheckedSocket(const QString &port)
{
    chip->setCheckedSocket(port);
}



