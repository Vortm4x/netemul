#include "device.h"

Device::Device(QObject *parent) :
    QObject(parent) , m_impl(0)
{
}
