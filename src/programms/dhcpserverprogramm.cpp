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
#include "dhcppacket.h"
#include "smartdevice.h"
#include "udpsocket.h"
#include <QMessageBox>

#include "dhcpservermodel.h"

int dhcpServerProgramm::myServerCount = 0;

dhcpServerProgramm::dhcpServerProgramm()
{
    myName = tr("DHCP server");       
    myServerCount++;
    myServerName = QString("Server%1").arg(myServerCount);
    myDemons.clear();
}

dhcpServerProgramm::~dhcpServerProgramm()
{
    myDemons.clear();
}

void dhcpServerProgramm::setDevice(SmartDevice *s)
{
    if ( s == 0 ) return;
    programmRep::setDevice(s);
    //receiver = new udpSocket(myDevice, SERVER_SOCKET);
    foreach ( Interface *i, myDevice->interfaces() ) {
        if ( i->isConnect() ) {
            dhcpDemon *demon = new dhcpDemon(i);
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

void dhcpServerProgramm::execute(QByteArray data)
{
//    if ( myDevice->adapter(myInterface)->ip().isEmpty() ) {
//        QMessageBox::warning(0,tr("Warning"),tr("Your DHCP server <i>%1</i> isn't configured.").arg(myServerName),
//                             QMessageBox::Ok, QMessageBox::Ok);
//        return;
//    }
    dhcpPacket packet(data);
    foreach ( dhcpDemon *demon, myDemons ) {
        if ( myDevice->adapter(demon->interfaceName())->ip() == packet.siaddr() ) {
            switch ( packet.type() ) {
                case dhcpPacket::DHCPDISCOVER : demon->executeDiscover(packet); break;
                case dhcpPacket::DHCPREQUEST : demon->executeRequest(packet); break;
                case dhcpPacket::DHCPDECLINE : demon->executeDecline(packet); break;
            }
        }
    }
}



void dhcpServerProgramm::incTime()
{
    foreach ( dhcpDemon *demon, myDemons ) {
        demon->incTime();
    }
}

void dhcpServerProgramm::showProperty()
{
    dhcpServerProperty *d = new dhcpServerProperty(myDevice);
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
    dhcpDemon *d = myDemons.at(0);
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
void dhcpServerProgramm::read(QDataStream &stream)
{
    programmRep::read(stream);
    dhcpDemon *d = new dhcpDemon(device()->interfaces().at(0));
    d->dhcpModel()->read(stream);
    d->read(stream);
}
//---------------------------------------------------



//---------------------------------------------------
clientState::clientState(staticDhcpRecord *rec)
{
    ip = rec->yiaddr;
    mac = rec->chaddr;
    mask = rec->mask;
    gateway = rec->gateway;
    time = rec->time;
}

