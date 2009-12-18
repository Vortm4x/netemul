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
#include "udppacket.h"
#include "dhcpservermodel.h"
#include <QMessageBox>

dhcpServerProgramm::dhcpServerProgramm()
{
    myName = tr("DHCP server");
    myDhcpModel = new dhcpServerModel;
    myDynamic = false;
    myTime = 300;
    myWaitingTime = 60;   
}

dhcpServerProgramm::~dhcpServerProgramm()
{
    delete myDhcpModel;
    delete receiver;
    clients.clear();
}

void dhcpServerProgramm::setDevice(smartDevice *s)
{
    if ( s == 0 ) return;
    programmRep::setDevice(s);
    receiver = new udpSocket(myDevice, SERVER_SOCKET);    
    foreach ( interface *i, myDevice->interfaces() ) {
        if ( i->isConnect() ) myInterface = i->name();
        break;
    }
    receiver->setBind("0.0.0.0");
    connect( receiver , SIGNAL(readyRead(QByteArray)), SLOT(execute(QByteArray)));
    connect( myDevice, SIGNAL(interfaceConnected(QString)), SLOT(checkInterface(QString)) );
}

void dhcpServerProgramm::setInterfaceName(QString inter)
{
    myInterface = inter;
}

void dhcpServerProgramm::checkInterface(QString port)
{
    if ( myInterface.isEmpty() ) setInterfaceName(port);
}

void dhcpServerProgramm::execute(QByteArray data)
{
    if ( myDevice->adapter(myInterface)->ip().isEmpty() ) {
        QMessageBox::warning(0,tr("Warning"),tr("Your DHCP server isn't configured."), QMessageBox::Ok,
                             QMessageBox::Ok);
        return;
    }
    dhcpPacket packet(data);
    switch ( packet.type() ) {
        case dhcpPacket::DHCPDISCOVER : executeDiscover(packet); break;
        case dhcpPacket::DHCPREQUEST : executeRequest(packet); break;
        case dhcpPacket::DHCPDECLINE : executeDecline(packet); break;
    }
}

void dhcpServerProgramm::executeDiscover(dhcpPacket packet)
{
    clientState *client = findClient( packet.xid() );
    if ( client && client->state == clientState::IN_USE ) return;
    client = chooseStatic(packet);
    if ( client ) {
        makeAnswer(client, dhcpPacket::DHCPOFFER);
        return;
    }
    if ( !myDynamic ) return;
    client = chooseDynamic(packet);
    if ( client ) makeAnswer(client,dhcpPacket::DHCPOFFER);
}

void dhcpServerProgramm::executeRequest(dhcpPacket packet)
{
    clientState *client = findClient( packet.xid() );
    if ( !client ) return;
    if ( packet.siaddr() != myDevice->adapter(myInterface)->ip() ) {
        clients.removeOne(client);
        delete client;
        return;
    }
    makeAnswer( client, dhcpPacket::DHCPACK );
}

void dhcpServerProgramm::executeDecline(dhcpPacket packet)
{
    clientState *client = findClient(packet.yiaddr());
    if ( !client ) return;    
    client->state = clientState::DECLINE;
}

clientState* dhcpServerProgramm::chooseStatic(dhcpPacket packet)
{
    staticDhcpRecord *rec = myDhcpModel->recordWithMac(packet.chaddr());
    if ( !rec ) return 0;
    clientState *client = new clientState(rec);
    if ( findClient(client->ip) ) return 0;
    client->requestTimer = 0;
    client->xid = packet.xid();
    clients << client;
    return client;
}

/*!
  Выбираем адрес из динамического диапазона.
  @return указатель на созданную запись.
  */
clientState* dhcpServerProgramm::chooseDynamic(dhcpPacket packet)
{
    clientState *cl = new clientState;
    cl->requestTimer = 0;
    clientState *c = findClient(packet.yiaddr());
    if ( !packet.yiaddr().isEmpty() && !c ) cl->ip = packet.yiaddr();
    else cl->ip = giveDynamicIp();
    if ( cl->ip.isEmpty() ) return NULL;
    cl->mac = packet.chaddr();
    cl->xid = packet.xid();
    cl->time = myTime;
    cl->mask = myMask;
    cl->gateway = myGateway;
    clients << cl;
    return cl;
}

/*!
  Создаем dhcp пакет.
  @param client - запись клиента на основе которой будем создавать пакет.
  @param state - тип отправляемого сообщения.
  @return созданный пакет.
  */
dhcpPacket dhcpServerProgramm::createDhcpPacket( clientState *client, int state ) const
{
    if ( state == dhcpPacket::DHCPOFFER ) client->state = clientState::WAIT_REQUEST;
    else client->state = clientState::IN_USE;
    dhcpPacket p;
    p.setType( state );
    p.setXid( client->xid );
    p.setChaddr( client->mac );
    p.setYiaddr( client->ip );
    p.setMask( client->mask );
    p.setGateway( client->gateway );
    p.setSiaddr( myDevice->adapter(myInterface)->ip() );
    p.setTime( client->time );
    return p;
}
//------------------------------------------------------------

void dhcpServerProgramm::sendDhcp(dhcpPacket packet) const
{
    udpPacket udp;
    udp.setSender( SERVER_SOCKET );
    udp.setReceiver( CLIENT_SOCKET );
    udp.pack( packet.toData() );
    ipPacket p( myDevice->adapter(myInterface)->ip(), ipAddress::full() );
    p.pack( udp.toData() );
    p.setUpProtocol( ipPacket::udp );
    myDevice->adapter(myInterface)->sendPacket(p);
}

void dhcpServerProgramm::makeAnswer(clientState *client, int type)
{
    dhcpPacket dhcp = createDhcpPacket(client,type);
    sendDhcp(dhcp);
}

/*! Ищет в списке клиента с данным идентификатрором и проверкой состояния записи.
  @param xid - идентификатрор.
  @return указатель на запись из списка, если xid совпали, или 0 в противном случае.
  */
clientState* dhcpServerProgramm::findClient(int xid) const
{
    foreach ( clientState *i, clients )
        if ( i->xid == xid && i->state != clientState::DECLINE ) return i;
    return 0;
}
//------------------------------------------------------------
/*! Ищет в списке клиента с данным ip-адресом.
  @param ip - адрес.
  @return указатель на запись из списка, если ip совпали, или 0 в противном случае.
  */
clientState* dhcpServerProgramm::findClient(ipAddress ip) const
{
    foreach ( clientState *i, clients )
        if ( i->ip == ip ) return i;
    return 0;
}
//--------------------------------------------------------------
/*!
  Выбирает ip-адрес из динамического диапазона
  @return выбранный адрес, или "0.0.0.0", если нет свободных адресов.
  */
ipAddress dhcpServerProgramm::giveDynamicIp() const
{
    bool isContains = false;
    quint32 i = myBeginIp.toInt();
    while ( i <= myEndIp.toInt() ) {
        isContains = myDhcpModel->containRecord( ipAddress(i) ) || findClient(ipAddress(i))
                     || myDevice->adapter(myInterface)->ip().toInt() == i;
        if ( isContains ) {
            i++;
            isContains = false;
        }
        else {
            return ipAddress(i);
        }
    }
    return ipAddress("0.0.0.0");
}
//-------------------------------------------------------------

void dhcpServerProgramm::incTime()
{
    bool canDelete = false;
    foreach ( clientState *i, clients ) {
        if ( i->state == clientState::WAIT_REQUEST ) {
            if ( ++i->requestTimer == myWaitingTime ) canDelete = true;
        }
        else if ( i->state == clientState::IN_USE && --i->time == 0 ) canDelete = true;
        if ( canDelete ) {
            canDelete = false;
            clients.removeOne(i);
            delete i;
        }
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
    myDhcpModel->write(stream);
    stream << myInterface;
    stream << myBeginIp;
    stream << myEndIp;
    stream << myMask;
    stream << myGateway;
    stream << myTime;
    stream << myDynamic;
    stream << myWaitingTime;
}
//---------------------------------------------------

void dhcpServerProgramm::writeXml(sceneXmlWriter &stream) const
{
    const QMetaObject *meta = metaObject();
    for ( int i = 1; i < meta->propertyCount(); i++ ){
        QMetaProperty temp = meta->property(i);
        stream.writeTextElement( temp.name(), temp.read(this).toString() );
    }
    stream.writeStartElement("statictable");
    myDhcpModel->writeXml(stream);
    stream.writeEndElement();
}

/*!
  Считывает отличительные черты из потока.
  @param stream - поток для чтения.
*/
void dhcpServerProgramm::read(QDataStream &stream)
{
    programmRep::read(stream);
    myDhcpModel->read(stream);
    stream >> myInterface;
    stream >> myBeginIp;
    stream >> myEndIp;
    stream >> myMask;
    stream >> myGateway;
    stream >> myTime;
    stream >> myDynamic;
    stream >> myWaitingTime;
}
//---------------------------------------------------

void dhcpServerProgramm::readXml(sceneXmlReader &stream)
{
    Q_ASSERT( stream.isStartElement() && stream.name() == "programm" );
    while ( !stream.atEnd() ) {
        stream.readNext();
        if ( stream.isEndElement() ) break;
        if ( property( stream.name().toString().toLocal8Bit() ).isValid() )
            setProperty( stream.name().toString().toLocal8Bit() , stream.readElementText() );
        if ( stream.name() == "statictable" )
            myDhcpModel->readXml(stream);
    }
}


//---------------------------------------------------
clientState::clientState(staticDhcpRecord *rec)
{
    ip = rec->yiaddr;
    mac = rec->chaddr;
    mask = rec->mask;
    gateway = rec->gateway;
    time = rec->time;
}

