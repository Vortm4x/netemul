#include "deviceimpl.h"

deviceImpl::deviceImpl()
{

}
//----------------------------------------------------------

void deviceImpl::write(QDataStream &stream) const
{
    stream << myNote;
}

void deviceImpl::read(QDataStream &stream)
{
    stream >> myNote;
}

void deviceImpl::setIp(const QString& , const QString&)
{
}

void deviceImpl::setMask(const QString&, const QString&)
{

}

void deviceImpl::setGateway(const QString&)
{
}

void deviceImpl::setSocketsCount(int)
{
}

