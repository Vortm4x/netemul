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

QString frame::toString() const
{
    QString temp;
    temp.append(QObject::tr("Ethernet, sender: %1 receiver: %2").arg(d->sender.toString()).arg(d->receiver.toString()));
    return temp;
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



