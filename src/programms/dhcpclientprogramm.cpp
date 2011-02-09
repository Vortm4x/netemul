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

DhcpClientProgram::DhcpClientProgram(QObject *parent) : Program(parent)
{
    myName = tr("DHCP client");
    myOfferTime = 60;
}

DhcpClientProgram::~DhcpClientProgram()
{
    qDeleteAll(myStates);
    // Make it
    //delete listener;
    myDevice->disposeSocket(listener);
}

void DhcpClientProgram::incTime()
{
    foreach ( InterfaceState *i , myStates ) {
        --i->time;
        if ( i->time <= 0 ) {
            switch ( i->state ) {
                case InterfaceState::CS_ALL_RIGHT: restartSession(i); break;
                case InterfaceState::CS_WAIT_VARIANT: sendDiscover( i->name ); break;
                case InterfaceState::CS_WAIT_RESPONSE: sendRequest(i->name ); break;
            }
        }
    }
}
/*!
  * Переопределяем функцию установки устройства чтобы соединиться со слотом.
  * @param s - указатель на устройство на которое установлена программа.
  */
void DhcpClientProgram::setDevice(SmartDevice *s)
{
    if ( s == 0 ) {
        foreach ( InterfaceState *i , myStates ) resetClient(i);
        return;
    }
    Program::setDevice(s);
    listener = s->openSocket( CLIENT_SOCKET , SmartDevice::UDP );
    listener->setBind("0.0.0.0");
    connect( listener , SIGNAL(readyRead(QByteArray)) , SLOT(processData(QByteArray)) );
    connect( s , SIGNAL(interfaceDeleted(QString)), SLOT(deleteInterface(QString)) );
    foreach ( InterfaceState *i , myStates ) {
        resetClient( i );
    }
}
//------------------------------------------------------

void DhcpClientProgram::resetClient(InterfaceState *session)
{
    myDevice->adapter(session->name)->setIp(IpAddress("0.0.0.0"));
    myDevice->adapter(session->name)->setMask(IpAddress("0.0.0.0"));
    myDevice->connectedNet( myDevice->adapter( session->name ) );
    myDevice->setGateway("0.0.0.0");
}

bool DhcpClientProgram::isUnderDhcpControl(const QString name) const
{
    foreach ( InterfaceState *i , myStates )
        if ( i->name == name ) {
            return true;
        }
    return false;
}

/*!
  Посылает Request серверу
  @param name - имя интерфейса
  */
void DhcpClientProgram::sendRequest(const QString &name)
{
    InterfaceState *t = stateAt(name);
    if ( !t ) return;
    if ( REPEAT_COUNT < ++t->count ) {
        restartSession(t);
        return;
    }
    t->time = myOfferTime;
    DhcpPacket message;
    message.setType( DhcpPacket::DHCPREQUEST );
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
void DhcpClientProgram::sendDiscover(const QString &name)
{ 
    InterfaceState *t = stateAt(name);
    if ( !t ) return;
    t->state = InterfaceState::CS_WAIT_VARIANT;
    t->time = myOfferTime;
    t->count = 0;
    DhcpPacket message;
    message.setType( DhcpPacket::DHCPDISCOVER );
    message.setXid(t->xid);
    message.setChaddr( myDevice->adapter(t->name)->mac() );
    if ( !t->lastIp.isEmpty() ) message.setYiaddr( t->lastIp );
    sendDhcpMessage(message,t);
}
//--------------------------------------------------------------
void DhcpClientProgram::sendDecLine(const QString &name)
{
    InterfaceState *t = stateAt(name);
    if ( !t ) return;
    DhcpPacket message;
    message.setType( DhcpPacket::DHCPDECLINE );
    message.setXid( t->xid );
    if ( !t->lastIp.isEmpty() ) message.setYiaddr( t->lastIp );
    message.setChaddr( myDevice->adapter( t->name )->mac() );
    sendDhcpMessage( message , t );
}
/*!
  Обрабатывает входящие данные.
  @param data - пришедщие данные.
  */
void DhcpClientProgram::processData(QByteArray data)
{
    DhcpPacket packet(data);
    switch ( packet.type() ) {
        case DhcpPacket::DHCPOFFER: receiveOffer(packet); break;
        case DhcpPacket::DHCPACK: receiveAck(packet); break;
    }
}
//---------------------------------------------------------------
/*!
  Начинает заново сессию
  @param session - указатель на сессию
  */
void DhcpClientProgram::restartSession(InterfaceState *session)
{
    resetClient(session);
    sendDiscover( session->name );
}
//---------------------------------------------------------------
/*!
  Обрабатывает входящее предложение настроек.
  @param packet - пакет с настройками.
  */
void DhcpClientProgram::receiveOffer(DhcpPacket packet)
{
    foreach ( InterfaceState *i , myStates )
        if ( i->xid == packet.xid() && i->state == InterfaceState::CS_WAIT_VARIANT ) {
            i->state = InterfaceState::CS_WAIT_RESPONSE;
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
void DhcpClientProgram::receiveAck(DhcpPacket packet)
{
    foreach ( InterfaceState *i , myStates )
        if ( i->xid == packet.xid() && i->state == InterfaceState::CS_WAIT_RESPONSE ) {
            i->state = InterfaceState::CS_ALL_RIGHT;
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
void DhcpClientProgram::sendDhcpMessage(DhcpPacket message, InterfaceState *state)
{
    if (!myDevice->adapter(state->name)->isConnect() ) return;
    UdpPacket udp;
    udp.setSender(CLIENT_SOCKET);
    udp.setReceiver(SERVER_SOCKET);
    udp.pack( message.toData() );
    IpPacket packet( myDevice->adapter(state->name)->ip() , IpAddress::full() );
    packet.pack( udp.toData() );
    packet.setUpProtocol( IpPacket::udp );
    myDevice->adapter(state->name)->sendPacket( packet);
}
//---------------------------------------------------------------
/*!
  * Показывает диалог программы.
  */
void DhcpClientProgram::showProperty()
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
InterfaceState* DhcpClientProgram::stateAt(const QString name)
{
    foreach ( InterfaceState *i , myStates )
        if ( i->name == name ) return i;
    return 0;
}
//--------------------------------------------------------------
QStringList DhcpClientProgram::interfacesList() const
{
    return myDevice->sockets();
}
/*!
  * Возвращаем иконку подключения, для указанного интерфейса.
  * @param имя интерефейса
  * @return иконка соединения
  */
QIcon DhcpClientProgram::isConnectSocketIcon(const QString &name) const
{
    return myDevice->isConnectSocketIcon(name);
}
//-----------------------------------------------------------------------
/*!
  * Вызывается когда у устройства удаляется интерфейс, если мы за
  * ним следим, то мы прекращаем это делать и удаляем его из списка.
  */
void DhcpClientProgram::deleteInterface(const QString name)
{
    InterfaceState *t = stateAt(name);
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
void DhcpClientProgram::observeInterface(const QString &name, bool b)
{
    if ( !myEnable ) return;
    InterfaceState *temp = stateAt(name);
    if ( temp ) {
        if ( b ) return;
        resetClient( temp );
        myStates.removeOne(temp);
        delete temp;
        return;
    }
    if ( !b ) return;
    InterfaceState *session = new InterfaceState;
    session->name = name;
    session->xid = qrand()%5000;
    session->time = 0;
    connect( myDevice->adapter(session->name) , SIGNAL(equalIpDetected()) , SLOT(onDetectEqualIp()) );
    myStates << session;
    sendDiscover(session->name);
}
//--------------------------------------------------------------------

void DhcpClientProgram::onDetectEqualIp()
{
    Interface *t = qobject_cast<Interface*>(sender());
    InterfaceState *client = 0;
    foreach ( InterfaceState *i , myStates )
        if ( myDevice->adapter(  i->name ) == t ) client = i;
    if ( !client ) return;
    sendDecLine(client->name);
    client->xid = qrand()%5000;
    client->lastIp.setIp("0.0.0.0");
    restartSession( client);
}

Qt::CheckState DhcpClientProgram::checkedState(const QString &name) const
{
    foreach ( InterfaceState *i , myStates )
        if ( i->name == name ) return Qt::Checked;
    return Qt::Unchecked;
}
/*!
  Записывает отличительные черты в поток.
  @param stream - поток для записи.
*/
void DhcpClientProgram::write(QDataStream &stream) const
{
    stream << DHCPClient;
    Program::write(stream);
    stream << myOfferTime;
    stream << myStates.size();
    foreach ( InterfaceState *i , myStates ) i->write(stream);
}
//---------------------------------------------------
/*!
  Считывает отличительные черты из потока.
  @param stream - поток для чтения.
*/
void DhcpClientProgram::read(QDataStream &stream)
{
    Program::read(stream);
    stream >> myOfferTime;
    int n;
    stream >> n;
    for ( int i = 0 ; i < n ; i++ ) {
        InterfaceState *temp = new InterfaceState;
        temp->read(stream);        
        temp->time = 0;
        temp->state = InterfaceState::CS_WAIT_VARIANT;
        myStates << temp;
    }
}
//---------------------------------------------------
QVariantList DhcpClientProgram::statesObjectList() const
{
    QVariantList list;
    foreach ( InterfaceState *i , myStates ) {
        QObject *o = new InterfaceStateObject(i);
        list << qVariantFromValue(o);
    }
    return list;
}

void DhcpClientProgram::addInterfaceStateObject(InterfaceStateObject *obj)
{
    InterfaceState *s = obj->object();
    s->state = InterfaceState::CS_WAIT_VARIANT;
    myStates << s;
    obj->deleteLater();
}

//---------------------------------------------------
//---------------------------------------------------
void InterfaceState::write(QDataStream &stream) const
{
    stream << xid << time << serverAddress << lastIp << name;
}

void InterfaceState::read(QDataStream &stream)
{
    stream >> xid >> time >> serverAddress >> lastIp >> name;
}



