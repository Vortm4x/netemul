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
#include "dhcpserverprogram.h"
#include "dhcpserverproperty.h"
#include "dhcppacket.h"
#include "smartdevice.h"
#include "udpsocket.h"
#include "socketfactory.h"
#include <QMessageBox>

#include "dhcpservermodel.h"

int DhcpServerProgram::myServerCount = 0;

DhcpServerProgram::DhcpServerProgram(QObject *parent) : Program(parent)
{
    myName = tr("DHCP server");       
    myServerCount++;
    myServerName = QString("Server%1").arg(myServerCount);
    myDaemons.clear();
    qDeleteAll(myDaemons);
}

DhcpServerProgram::~DhcpServerProgram()
{
    myDaemons.clear();
    qDeleteAll(myDaemons);
}

void DhcpServerProgram::setDevice(SmartDevice *s)
{
    if ( s == 0 ) return;
    Program::setDevice(s);
    foreach ( Interface *i, myDevice->interfaces() ) {
        if ( i->isConnect() ) {
            DhcpDaemon *daemon = new DhcpDaemon(i, this);
            myDaemons.insert(i, daemon);
        }
    }
    connect( myDevice, SIGNAL(interfaceConnected(QString)), SLOT(checkInterfaceOnConnect(QString)) );
    connect( myDevice, SIGNAL(interfaceDeleted(QString)), SLOT(checkInterfaceOnDelete(QString)) );
}

void DhcpServerProgram::checkInterfaceOnConnect(QString port)
{
    Interface *i = myDevice->adapter(port);
    if ( !myDaemons.contains(i) ) {
        DhcpDaemon *daem = new DhcpDaemon(i, this);
        myDaemons.insert(i, daem);
    }
}

void DhcpServerProgram::checkInterfaceOnDelete(QString port)
{
    Interface *i = myDevice->adapter(port);
    if ( !myDaemons.contains(i) ) {
        DhcpDaemon *daem = myDaemons.take(i);
        delete daem;
    }
}

void DhcpServerProgram::incTime()
{
    foreach ( DhcpDaemon *demon, myDaemons.values() ) {
        demon->incTime();
    }
}

void DhcpServerProgram::showProperty()
{
    DhcpServerSetting *setting = new DhcpServerSetting(this);
    DhcpServerProperty *d = new DhcpServerProperty(setting);
    d->exec();
}

DhcpDaemon* DhcpServerProgram::daemonOf(Interface *inter)
{
    return myDaemons.value(inter);
}

void DhcpServerProgram::addDhcpDaemon(DhcpDaemon *daemon)
{
    Interface *i = daemon->interface();
    if ( !i ) {
        i = myDevice->adapter(daemon->interfaceName());
        daemon->setInterface(i);
    }
    myDaemons.insert(i, daemon);
}

/*!
  Записывает отличительные черты в поток.
  @param stream - поток для записи.
*/
void DhcpServerProgram::write(QDataStream &stream) const
{
    stream << DHCPServer;
    Program::write(stream);
    DhcpDaemon *d = myDaemons.values().at(0);
    d->dhcpModel()->write(stream);
    stream << d->interfaceName();
    stream << d->beginIp();
    stream << d->endIp();
    stream << d->mask();
    stream << d->gateway();
    stream << d->time();
    stream << d->dynamic();
    stream << d->waitingTime();
}
//---------------------------------------------------

/*!
  Считывает отличительные черты из потока.
  @param stream - поток для чтения.
*/
void DhcpServerProgram::read(QDataStream &stream)
{
    Program::read(stream);
    DhcpDaemon *d = new DhcpDaemon(device()->interfaces().at(0));
    d->dhcpModel()->read(stream);
    d->read(stream);
}
//---------------------------------------------------

