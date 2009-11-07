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

void boxDevice::writeXml(QXmlStreamWriter &stream) const
{
    stream.writeStartElement("boxchip");
    deviceImpl::writeXml(stream);
    stream.writeEndElement();
}

void boxDevice::read(QDataStream &stream)
{
    deviceImpl::read(stream);
    chip->read(stream);
}

void boxDevice::readXml(QXmlStreamReader &stream)
{
    qDebug(" - %s", stream.name().toString().toAscii().data() );
    while ( !stream.atEnd() ) {
        stream.readNext();
        if ( stream.isEndElement() ) break;
        if ( stream.name() == "deviceimpl" ) deviceImpl::readXml(stream);
    }
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

QIcon boxDevice::isConnectSocketIcon(const QString &socket) const
{
    if ( isConnectSocket(socket) ) return QIcon(":/im/images/ok.png");
    return QIcon(":/im/images/not.png");
}

void boxDevice::setCheckedSocket(const QString &port)
{
    chip->setCheckedSocket(port);
}



