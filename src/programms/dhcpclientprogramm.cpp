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
#include "dhcpclientprogramm.h"
#include "smartdevice.h"
#include "udpsocket.h"
#include "dhcpclientproperty.h"
#include "udppacket.h"

dhcpClientProgramm::dhcpClientProgramm()
{
    myName = tr("DHCP client");
    myOfferTime = 60;
}

dhcpClientProgramm::~dhcpClientProgramm()
{
    qDeleteAll(myStates);
    delete listener;
}

void dhcpClientProgramm::incTime()
{
    foreach ( interfaceState *i , myStates ) {
        --i->time;
        if ( i->time <= 0 ) {
            switch ( i->state ) {
                case interfaceState::CS_ALL_RIGHT: restartSession(i); break;
                case interfaceState::CS_WAIT_VARIANT: sendDiscover( i->name ); break;
                case interfaceState::CS_WAIT_RESPONSE: sendRequest(i->name ); break;
            }
        }
    }
}
/*!
  * Переопределяем функцию установки устройства чтобы соединиться со слотом.
  * @param s - указатель на устройство на которое установлена программа.
  */
void dhcpClientProgramm::setDevice(smartDevice *s)
{
    if ( s == 0 ) {
        foreach ( interfaceState *i , myStates ) resetClient(i);
        return;
    }
    programmRep::setDevice(s);
    listener = new udpSocket(s,CLIENT_SOCKET);
    listener->setBind("0.0.0.0");
    connect( listener , SIGNAL(readyRead(QByteArray)) , SLOT(processData(QByteArray)) );
    connect( s , SIGNAL(interfaceDeleted(QString)), SLOT(deleteInterface(QString)) );
    foreach ( interfaceState *i , myStates ) {
        resetClient( i );
    }
}
//------------------------------------------------------

void dhcpClientProgramm::resetClient(interfaceState *session)
{
    myDevice->adapter(session->name)->setIp(ipAddress("0.0.0.0"));
    myDevice->adapter(session->name)->setMask(ipAddress("0.0.0.0"));
    myDevice->connectedNet( myDevice->adapter( session->name ) );
    myDevice->setGateway("0.0.0.0");
}

bool dhcpClientProgramm::isUnderDhcpControl(const QString name) const
{
    foreach ( interfaceState *i , myStates )
        if ( i->name == name ) {
            return true;
        }
    return false;
}

/*!
  Посылает Request серверу
  @param name - имя интерфейса
  */
void dhcpClientProgramm::sendRequest(const QString &name)
{
    interfaceState *t = stateAt(name);
    if ( !t ) return;
    if ( REPEAT_COUNT < ++t->count ) {
        restartSession(t);
        return;
    }
    t->time = myOfferTime;
    dhcpPacket message;
    message.setType( dhcpPacket::DHCPREQUEST );
    message.setXid( t->xid );
    message.setChaddr( myDevice->adapter(t->name)->mac() );
    message.setSiaddr( t->serverAddress );
    sendDhcpMessage(message,t);
}
//------------------------------------------------------
/*!
  * Посылает запрос на получение настроек.
  * @param name - имя интерфейса.
  */
void dhcpClientProgramm::sendDiscover(const QString &name)
{ 
    interfaceState *t = stateAt(name);
    if ( !t ) return;
    t->state = interfaceState::CS_WAIT_VARIANT;
    t->time = myOfferTime;
    t->count = 0;
    dhcpPacket message;
    message.setType( dhcpPacket::DHCPDISCOVER );
    message.setXid(t->xid);
    message.setChaddr( myDevice->adapter(t->name)->mac() );
    if ( !t->lastIp.isEmpty() ) message.setYiaddr( t->lastIp );
    sendDhcpMessage(message,t);
}
//--------------------------------------------------------------
void dhcpClientProgramm::sendDecLine(const QString &name)
{
    interfaceState *t = stateAt(name);
    if ( !t ) return;
    dhcpPacket message;
    message.setType( dhcpPacket::DHCPDECLINE );
    message.setXid( t->xid );
    if ( !t->lastIp.isEmpty() ) message.setYiaddr( t->lastIp );
    message.setChaddr( myDevice->adapter( t->name )->mac() );
    sendDhcpMessage( message , t );
}
/*!
  Обрабатывает входящие данные.
  @param data - пришедщие данные.
  */
void dhcpClientProgramm::processData(QByteArray data)
{
    dhcpPacket packet(data);
    switch ( packet.type() ) {
        case dhcpPacket::DHCPOFFER: receiveOffer(packet); break;
        case dhcpPacket::DHCPACK: receiveAck(packet); break;
    }
}
//---------------------------------------------------------------
/*!
  Начинает заново сессию
  @param session - указатель на сессию
  */
void dhcpClientProgramm::restartSession(interfaceState *session)
{
    resetClient(session);
    sendDiscover( session->name );
}
//---------------------------------------------------------------
/*!
  Обрабатывает входящее предложение настроек.
  @param packet - пакет с настройками.
  */
void dhcpClientProgramm::receiveOffer(dhcpPacket packet)
{
    foreach ( interfaceState *i , myStates )
        if ( i->xid == packet.xid() && i->state == interfaceState::CS_WAIT_VARIANT ) {
            i->state = interfaceState::CS_WAIT_RESPONSE;
            i->serverAddress = packet.siaddr();
            sendRequest( i->name );
            return;
        }
}
//---------------------------------------------------------------
/*!
  Обрабатывает вхоодящий АСК.
  @param packet - ack пакет
  */
void dhcpClientProgramm::receiveAck(dhcpPacket packet)
{
    foreach ( interfaceState *i , myStates )
        if ( i->xid == packet.xid() && i->state == interfaceState::CS_WAIT_RESPONSE ) {
            i->state = interfaceState::CS_ALL_RIGHT;
            myDevice->adapter(i->name)->setIp( packet.yiaddr() );
            myDevice->adapter(i->name)->setMask( packet.mask() );
            myDevice->connectedNet(myDevice->adapter(i->name));
            myDevice->setGateway( packet.gateway().toString() );
            myDevice->updateView();
            i->time = packet.time();
            i->lastIp = packet.yiaddr();
            myDevice->adapter(i->name)->sendArpRequest( packet.yiaddr() );
            return;
        }
}
//---------------------------------------------------------------
/*!
  Отправляет пакет с нужного интерфейса.
  @param message - пакет.
  @param state - поток-отправитель.
  */
void dhcpClientProgramm::sendDhcpMessage(dhcpPacket message, interfaceState *state)
{
    if (!myDevice->adapter(state->name)->isConnect() ) return;
    udpPacket udp;
    udp.setSender(CLIENT_SOCKET);
    udp.setReceiver(SERVER_SOCKET);
    udp.pack( message.toData() );
    ipPacket packet( myDevice->adapter(state->name)->ip() , ipAddress::full() );
    packet.pack( udp.toData() );
    packet.setUpProtocol( ipPacket::udp );
    myDevice->adapter(state->name)->sendPacket( packet);
}
//---------------------------------------------------------------
/*!
  * Показывает диалог программы.
  */
void dhcpClientProgramm::showProperty()
{
    dhcpClientProperty *d = new dhcpClientProperty;
    d->setProgramm(this);
    d->exec();
}
//---------------------------------------------------------------
/*!
  * Ищет указанный сеанс связи для интерфейса.
  * @param name - имя интерфейса.
  * @return указатель на сеанс, если такого нет то 0
  */
interfaceState* dhcpClientProgramm::stateAt(const QString name)
{
    foreach ( interfaceState *i , myStates )
        if ( i->name == name ) return i;
    return 0;
}
//--------------------------------------------------------------
QStringList dhcpClientProgramm::interfacesList() const
{
    return myDevice->sockets();
}
/*!
  * Возвращаем иконку подключения, для указанного интерфейса.
  * @param имя интерефейса
  * @return иконка соединения
  */
QIcon dhcpClientProgramm::isConnectSocketIcon(const QString &name) const
{
    return myDevice->isConnectSocketIcon(name);
}
//-----------------------------------------------------------------------
/*!
  * Вызывается когда у устройства удаляется интерфейс, если мы за
  * ним следим, то мы прекращаем это делать и удаляем его из списка.
  */
void dhcpClientProgramm::deleteInterface(const QString name)
{
    interfaceState *t = stateAt(name);
    if ( !t ) return;
    myStates.removeOne(t);
    delete t;
}
//--------------------------------------------------------------------
/*!
  * Если интерфейс еще не добавлен под наблюдение, то добавляем его.
  * @param name - имя интерфейса.
  * @param b - включить или выключить наблюдение.
  */
void dhcpClientProgramm::observeInterface(const QString &name, bool b)
{
    if ( !myEnable ) return;
    interfaceState *temp = stateAt(name);
    if ( temp ) {
        if ( b ) return;
        resetClient( temp );
        myStates.removeOne(temp);
        delete temp;
        return;
    }
    if ( !b ) return;
    interfaceState *session = new interfaceState;
    session->name = name;
    session->xid = qrand()%5000;
    session->time = 0;
    connect( myDevice->adapter(session->name) , SIGNAL(equalIpDetected()) , SLOT(onDetectEqualIp()) );
    myStates << session;
    sendDiscover(session->name);
}
//--------------------------------------------------------------------

void dhcpClientProgramm::onDetectEqualIp()
{
    interface *t = qobject_cast<interface*>(sender());
    interfaceState *client = 0;
    foreach ( interfaceState *i , myStates )
        if ( myDevice->adapter(  i->name ) == t ) client = i;
    if ( !client ) return;
    sendDecLine(client->name);
    client->xid = qrand()%5000;
    client->lastIp.setIp("0.0.0.0");
    restartSession( client);
}

Qt::CheckState dhcpClientProgramm::checkedState(const QString &name) const
{
    foreach ( interfaceState *i , myStates )
        if ( i->name == name ) return Qt::Checked;
    return Qt::Unchecked;
}
/*!
  Записывает отличительные черты в поток.
  @param stream - поток для записи.
*/
void dhcpClientProgramm::write(QDataStream &stream) const
{
    stream << DHCPClient;
    programmRep::write(stream);
    stream << myOfferTime;
    stream << myStates.size();
    foreach ( interfaceState *i , myStates ) i->write(stream);
}
//---------------------------------------------------
/*!
  Считывает отличительные черты из потока.
  @param stream - поток для чтения.
*/
void dhcpClientProgramm::read(QDataStream &stream)
{
    programmRep::read(stream);
    stream >> myOfferTime;
    int n;
    stream >> n;
    for ( int i = 0 ; i < n ; i++ ) {
        interfaceState *temp = new interfaceState;
        temp->read(stream);        
        temp->time = 0;
        temp->state = interfaceState::CS_WAIT_VARIANT;
        myStates << temp;
    }
}

void dhcpClientProgramm::writeXml(sceneXmlWriter &stream) const
{
    const QMetaObject *meta = metaObject();
    for ( int i = 1 ; i < meta->propertyCount() ; i++ ) {
        QMetaProperty temp = meta->property(i);
        stream.writeTextElement( temp.name() , temp.read(this).toString() );
    }
    foreach ( interfaceState *i , myStates ) {
        stream.writeStartElement("interface");
        i->writeXml(stream);
        stream.writeEndElement();
    }
}

void dhcpClientProgramm::readXml(sceneXmlReader &stream)
{
    Q_ASSERT( stream.isStartElement() && stream.name() == "programm" );
    while ( !stream.atEnd() ) {
        stream.readNext();
        if ( stream.isEndElement() ) break;
        if ( property( qPrintable(stream.name().toString()) ).isValid() ) {
            setProperty( qPrintable(stream.name().toString() ) , stream.readElementText() );
        } else if ( stream.name() == "interface" ) {
            interfaceState *temp = new interfaceState;
            temp->readXml(stream);
            temp->state = interfaceState::CS_WAIT_VARIANT;
            myStates << temp;
        }
    }
}

//---------------------------------------------------
//---------------------------------------------------
void interfaceState::write(QDataStream &stream) const
{
    stream << xid << time << serverAddress << lastIp << name;
}

void interfaceState::writeXml(sceneXmlWriter &stream) const
{
    stream.writeTextElement("xid", QString::number(xid) );
    stream.writeTextElement("time", QString::number(time) );
    stream.writeTextElement("server", serverAddress.toString() );
    stream.writeTextElement("lastip", lastIp.toString() );
    stream.writeTextElement("name", name);
}

void interfaceState::read(QDataStream &stream)
{
    stream >> xid >> time >> serverAddress >> lastIp >> name;
}

void interfaceState::readXml(sceneXmlReader &stream)
{
    Q_ASSERT( stream.isStartElement() && stream.name() == "interface" );
    while ( !stream.atEnd() ) {
        stream.readNext();
        if ( stream.isEndElement() ) break;
        if ( stream.name() == "xid" ) xid = stream.readElementText().toInt();
        else if ( stream.name() == "time" ) time = stream.readElementText().toInt();
        else if ( stream.name() == "server" ) serverAddress.setIp( stream.readElementText() );
        else if ( stream.name() == "lastip" ) lastIp.setIp( stream.readElementText() );
        else if ( stream.name() == "name" ) name = stream.readElementText();
    }
}


