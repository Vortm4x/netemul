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
#include "dhcpserverprogramm.h"
#include "dhcpserverproperty.h"
#include "smartdevice.h"
#include "udpsocket.h"

dhcpServerProgramm::dhcpServerProgramm()
{
    myName = tr("DHCP server");
}

void dhcpServerProgramm::setDevice(smartDevice *s)
{
    programmRep::setDevice(s);
    receiver = new udpSocket(device, SERVER_SOCKET);
    receiver->setBind("0.0.0.0");
    connect( receiver , SIGNAL(readyRead(QByteArray)), SLOT(execute(QByteArray)));
}

void dhcpServerProgramm::execute(QByteArray data)
{
    QString str;
    QDataStream s(data);
    s >> str;

}

bool dhcpServerProgramm::containRecord(staticRecord *rec)
{
    if ( statics.isEmpty() ) return false;
    foreach (staticRecord *i, statics)
        if ( i == rec ) return true;
    return false;
}

void dhcpServerProgramm::addStaticRecord(staticRecord *rec)
{
    statics << rec;
}

void dhcpServerProgramm::showProperty()
{
    dhcpServerProperty *d = new dhcpServerProperty;
    d->setProgramm(this);
    d->exec();
}

/*!
  Записывает отличительные черты в поток.
  @param stream - поток для записи.
*/
void dhcpServerProgramm::write(QDataStream &stream) const
{
    stream << DHCPServer;
    programmRep::write(stream);
}
//---------------------------------------------------
/*!
  Считывает отличительные черты из потока.
  @param stream - поток для чтения.
*/
void dhcpServerProgramm::read(QDataStream &stream)
{
    programmRep::read(stream);
}
//---------------------------------------------------
