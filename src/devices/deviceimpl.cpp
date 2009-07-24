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

void deviceImpl::setIp(const QString &a , const QString &ip)
{
    Q_UNUSED(a);
    Q_UNUSED(ip);
}

void deviceImpl::setMask(const QString &a , const QString &ip)
{
    Q_UNUSED(a);
    Q_UNUSED(ip);
}

void deviceImpl::setGateway(const QString &str)
{
    Q_UNUSED(str)
}

void deviceImpl::setSocketsCount(int n)
{
    Q_UNUSED(n)
}

