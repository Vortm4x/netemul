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
    myDemons.clear();
    qDeleteAll(myDemons);
}

DhcpServerProgram::~DhcpServerProgram()
{
    myDevice->disposeSocket(receiver);
    myDemons.clear();
    qDeleteAll(myDemons);
}

void DhcpServerProgram::setDevice(SmartDevice *s)
{
    if ( s == 0 ) return;
    Program::setDevice(s);
    receiver = myDevice->openSocket(DhcpDemon::SERVER_SOCKET, SocketFactory::UDP);
    //receiver = new udpSocket(myDevice, SERVER_SOCKET);
    foreach ( Interface *i, myDevice->interfaces() ) {
        if ( i->isConnect() ) {
            DhcpDemon *demon = new DhcpDemon(i);
            myDemons << demon;
        }
    }
    receiver->setBind("0.0.0.0");
    connect( receiver , SIGNAL(readyRead(QByteArray)), SLOT(execute(QByteArray)));
    connect( myDevice, SIGNAL(interfaceConnected(QString)), SLOT(checkInterface(QString)) );
}

//void dhcpServerProgramm::checkInterface(QString port)
//{
//    if ( myInterface.isEmpty() ) setInterfaceName(port);
//}

void DhcpServerProgram::execute(QByteArray data)
{
    DhcpPacket packet(data);
    foreach ( DhcpDemon *demon, myDemons ) {
        IpAddress deviceIp = myDevice->adapter(demon->interfaceName())->ip();
        if ( deviceIp.isEmpty() ) {
            QMessageBox::warning(0,tr("Warning"),
                 tr("Your DHCP server <i>%1</i> isn't configured.").arg(myServerName),
                 QMessageBox::Ok, QMessageBox::Ok);
            return;
        }

        if ( deviceIp == packet.siaddr() ) {
            switch ( packet.type() ) {
                case DhcpPacket::DHCPDISCOVER :
                    demon->executeDiscover(packet);
                    break;
                case DhcpPacket::DHCPREQUEST :
                    demon->executeRequest(packet);
                    break;
                case DhcpPacket::DHCPDECLINE :
                    demon->executeDecline(packet);
                    break;
            }
        }
    }
}



void DhcpServerProgram::incTime()
{
    foreach ( DhcpDemon *demon, myDemons ) {
        demon->incTime();
    }
}

void DhcpServerProgram::showProperty()
{
    DhcpServerProperty *d = new DhcpServerProperty(myDevice);
    d->setProgramm(this);
    d->exec();
}

/*!
  Записывает отличительные черты в поток.
  @param stream - поток для записи.
*/
void DhcpServerProgram::write(QDataStream &stream) const
{
    stream << DHCPServer;
    Program::write(stream);
    DhcpDemon *d = myDemons.at(0);
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
    DhcpDemon *d = new DhcpDemon(device()->interfaces().at(0));
    d->dhcpModel()->read(stream);
    d->read(stream);
}
//---------------------------------------------------



//---------------------------------------------------
//СlientState::СlientState(StaticDhcpRecord *rec)
//{
//    ip = rec->yiaddr;
//    mac = rec->chaddr;
//    mask = rec->mask;
//    gateway = rec->gateway;
//    time = rec->time;
//}

