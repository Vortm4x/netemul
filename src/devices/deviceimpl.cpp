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

