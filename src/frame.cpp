/****************************************************************************************
** NetEmul - program for simulating computer networks.
** Copyright © 2009 Semenov Pavel and Omilaeva Anastasia
**
** NetEmul is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** NetEmul is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with the NetEmul; if not, write to the Free
** Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
** 02111-1307 USA.
****************************************************************************************/
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



