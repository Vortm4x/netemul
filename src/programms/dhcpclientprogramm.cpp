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
}

dhcpClientProgramm::~dhcpClientProgramm()
{
    qDeleteAll(states);
    delete listener;
}

void dhcpClientProgramm::incTime()
{
    foreach ( interfaceState *i , states ) {
        --i->time;
        //qDebug("%d - state: %d",i->time , i->state);
        switch ( i->state ) {
            case interfaceState::CS_ALL_RIGHT: if ( i->time == 0 ) restartSession(i); break;
            case interfaceState::CS_WAIT_VARIANT: if ( i->time == 0 ) sendDiscover( i->name ); break;
        }
    }
}
/*!
  * Переопределяем функцию установки устройства чтобы соединиться со слотом.
  * @param s - указатель на устройство на которое установлена программа.
  */
void dhcpClientProgramm::setDevice(smartDevice *s)
{
    programmRep::setDevice(s);
    listener = new udpSocket(s,CLIENT_SOCKET);
    listener->setBind("0.0.0.0");
    connect( listener , SIGNAL(readyRead(QByteArray)) , SLOT(processData(QByteArray)) );
    connect( s , SIGNAL(interfaceDeleted(QString)), SLOT(deleteInterface(QString)) );
}
//------------------------------------------------------
/*!
  Посылает Request серверу
  @param name - имя интерфейса
  */
void dhcpClientProgramm::sendRequest(const QString &name)
{
    interfaceState *t = stateAt(name);
    if ( !t ) return;
    dhcpPacket message;
    message.setType( dhcpPacket::DHCPREQUEST );
    message.setXid( t->xid );
    message.setChaddr( t->adapter->mac() );
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
    t->time = MINUTE;
    dhcpPacket message;
    message.setType( dhcpPacket::DHCPDISCOVER );
    message.setXid(t->xid);
    message.setChaddr( t->adapter->mac() );
    sendDhcpMessage(message,t);
}
//--------------------------------------------------------------
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
  @param state - указатель на сессию
  */
void dhcpClientProgramm::restartSession(interfaceState *state)
{
    qFatal("Implement it!");
}
//---------------------------------------------------------------
/*!
  Обрабатывает входящее предложение настроек.
  @param packet - пакет с настройками.
  */
void dhcpClientProgramm::receiveOffer(dhcpPacket packet)
{
    foreach ( interfaceState *i , states )
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
    foreach ( interfaceState *i , states )
        if ( i->xid == packet.xid() && i->state == interfaceState::CS_WAIT_RESPONSE ) {
            i->state = interfaceState::CS_ALL_RIGHT;
            i->adapter->setIp( packet.yiaddr() );
            i->adapter->setMask( packet.mask() );
            device->connectedNet(i->adapter);
            device->setGateway( packet.gateway().toString() );
            i->time = packet.time();
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
    udpPacket udp;
    udp.setSender(CLIENT_SOCKET);
    udp.setReceiver(SERVER_SOCKET);
    udp.pack( message.toData() );
    ipPacket packet( state->adapter->ip() , ipAddress::full() );
    packet.pack( udp.toData() );
    packet.setUpProtocol( ipPacket::udp );
    state->adapter->sendPacket( packet);
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
    foreach ( interfaceState *i , states )
        if ( i->name == name ) return i;
    return 0;
}
//--------------------------------------------------------------
QStringList dhcpClientProgramm::interfacesList() const
{
    return device->sockets();
}
/*!
  * Возвращаем иконку подключения, для указанного интерфейса.
  * @param имя интерефейса
  * @return иконка соединения
  */
QIcon dhcpClientProgramm::isConnectSocketIcon(const QString &name) const
{
    return device->isConnectSocketIcon(name);
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
    states.removeOne(t);
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
        states.removeOne(temp);
        delete temp;
        return;
    }
    interfaceState *session = new interfaceState;
    session->name = name;
    session->xid = qrand()%5000;
    session->state = interfaceState::CS_WAIT_VARIANT;
    session->adapter = device->adapter(name);
    session->time = 0;
    states << session;
    sendDiscover(session->name);
}
//--------------------------------------------------------------------
Qt::CheckState dhcpClientProgramm::checkedState(const QString &name) const
{
    foreach ( interfaceState *i , states )
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
}
//---------------------------------------------------
/*!
  Считывает отличительные черты из потока.
  @param stream - поток для чтения.
*/
void dhcpClientProgramm::read(QDataStream &stream)
{
    programmRep::read(stream);
}
//---------------------------------------------------
