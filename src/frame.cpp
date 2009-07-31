#include "frame.h"

frame::frame(const QByteArray &b)
{
    d = new frameData;
    QDataStream s(b);
    s >> d->different >> d->sender >> d->receiver >> d->type  >> d->data;
}

QByteArray frame::toData() const
{
    return d->toArray();
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------

frameData::frameData()
{
    different = frame::normal;
}

QByteArray frameData::toArray() const
{
    QByteArray t;
    QDataStream s(&t,QIODevice::WriteOnly);
    s << different << sender << receiver << type << data;
    return t;
}



