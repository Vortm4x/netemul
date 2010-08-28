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
#include "boxdevice.h"
#include "logdialog.h"

BoxDevice::BoxDevice(QObject *parent) : DeviceImpl(parent)
{
}

BoxDevice::~BoxDevice()
{
}

void BoxDevice::write(QDataStream &stream) const
{
    DeviceImpl::write(stream);
    chip->write(stream);
}

void BoxDevice::writeXmlImpl(sceneXmlWriter &stream) const
{
    Q_UNUSED(stream)
//    deviceImpl::writeXml(stream);
}

void BoxDevice::read(QDataStream &stream)
{
    DeviceImpl::read(stream);
    chip->read(stream);
}

void BoxDevice::readXmlImpl(SceneXmlReader &stream)
{
    Q_UNUSED(stream)
//    Q_ASSERT( stream.isStartElement() && stream.name() == "impl");
//    while ( !stream.atEnd() ) {
//        stream.readNext();
//        if ( stream.isEndElement() ) break;
//        if ( stream.name() == "deviceimpl" ) deviceImpl::readXml(stream);
//    }
}

void BoxDevice::deciSecondTimerEvent()
{
    chip->deciSecondTimerEvent();
}

void BoxDevice::showLogDialog(logDialog *log) const
{
    connect( log , SIGNAL(changeInterface(QString)) , this , SLOT(setCheckedSocket(QString)) );
    connect( chip , SIGNAL(receiveData(frame,QString)) , log , SLOT(receiveData(frame,QString)) );
    connect( chip , SIGNAL(sendData(frame,QString)) , log , SLOT(sendData(frame,QString)) );
}

QIcon BoxDevice::isConnectSocketIcon(const QString &socket) const
{
    if ( isConnectSocket(socket) ) return QIcon(":/im/images/ok.png");
    return QIcon(":/im/images/not.png");
}

void BoxDevice::setCheckedSocket(const QString &port)
{
    chip->setCheckedSocket(port);
}



