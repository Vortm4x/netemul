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
#include <QtCore/QtDebug>
#include "smartdevice.h"
#ifndef __TESTING__
#include "routeeditor.h"
#include "adapterproperty.h"
#include "tablearp.h"
#include "programmdialog.h"
#include "logdialog.h"
#endif
#include "routemodel.h"
#include "tcpsocket.h"
#include "udpsocket.h"
#include "dhcpclientprogramm.h"


smartDevice::smartDevice() : myRouter(false)
{
    myReady = false;
    isDirty = true;
    myRouteTable = new routeModel(this);
    connect( myRouteTable , SIGNAL(recordAdding(routeRecord*,int)) , SLOT(tableChanged(routeRecord*,int)));
    connect( myRouteTable , SIGNAL(recordDeleting(routeRecord*,int)) , SLOT(tableChanged(routeRecord*,int)));
}

smartDevice::~smartDevice()
{
    myProgramms.clear();
    qDeleteAll(mySockets);
    qDeleteAll(myInterfaces);
}

const interface* smartDevice::adapter(const QString &s) const
{
    for ( int i = 0 ; i < myInterfaces.size() ; i++ )
        if ( myInterfaces.at(i)->name() == s ) return myInterfaces.at(i);
    return 0;
}

interface* smartDevice::adapter(const QString &name)
{
    for ( int i = 0 ; i < myInterfaces.size() ; i++ )
        if ( myInterfaces.at(i)->name() == name ) return myInterfaces[i];
    return 0;
}

interface* smartDevice::addInterface(const QString &name)
{
    interface *t = new interface(name);
    myInterfaces << t;
    connect( t, SIGNAL(receivedPacket(ipPacket)) , SLOT(receivePacket(ipPacket)) );
    return t;
}

interface* smartDevice::ipToAdapter(const ipAddress a)
{
    for ( int i = 0 ; i < myInterfaces.size() ; i++ )
        if ( myInterfaces.at(i)->ip() == a ) return myInterfaces[i];
    return NULL;
}

void smartDevice::receivePacket(ipPacket p)
{
    if ( p.decTtl() == 0 ) return;
    interface *f = qobject_cast<interface*>(sender());
    if ( p.receiver() == f->ip() || p.isBroadcast(f->mask()) ) treatPacket(p);
    else routePacket(p);
}
/*!
  Маршрутизирует пакет.
  @param p - указатель на пакет.
*/
void smartDevice::routePacket(ipPacket &p)
{
    if ( !myRouter ) return; // Выходим если нет маршрутизации.
    routeRecord *t = myRouteTable->recordAt(p.receiver());
    if ( !t ) return;
    ipAddress gw;
    if ( t->out != t->gateway ) gw = t->gateway;
    ipToAdapter(t->out)->sendPacket(p,gw);
}
//---------------------------------------------
/*!
  Вызывается при подключении или отключении сети от устройства, а также
  при смене ip-адреса или маски подсети.
  @param p - порт на котором произошло событие;
*/
void smartDevice::connectedNet(interface *p)
{
    checkReady();    
    ipAddress ip = p->ip();
    ipAddress mask = p->mask();
    if ( ip.isEmpty() || mask.isEmpty() ) return;
    myRouteTable->checkConnectedNet(ip, mask, p->isConnect());
}
//------------------------------------------------------------

void smartDevice::addConnection(const QString &port,cableDev *c)
{
    adapter(port)->setConnect(true,c);
    connectedNet(adapter(port));
}

void smartDevice::deleteConnection(cableDev *c)
{
    foreach ( interface *i , myInterfaces )
        if ( i->isCableConnect(c) ) {
            i->setConnect(false,0);
            connectedNet(i);
            return;
        }
}

/*!
  Записывает устройство в поток данных.
  @param stream - ссылка на поток.
*/
void smartDevice::write(QDataStream &stream) const
{
    deviceImpl::write(stream);
    stream << myInterfaces.size(); // Количество сокетов
    for ( int i = 0 ; i < myInterfaces.size() ; i++ )
        myInterfaces[i]->write(stream);
    stream << myRouter; // Включена или нет маршрутизация.
    myRouteTable->write(stream);
    stream << myProgramms.count(); // Количество программ.
    foreach ( programm i , myProgramms )  // И сами программы.
        stream << i;
}
//-------------------------------------------------
void smartDevice::read(QDataStream &stream)
{
    deviceImpl::read(stream);
    int n,i;
    stream >> n;
    qDeleteAll(myInterfaces);
    myInterfaces.clear();
    for (i = 0; i < n ; i++) {
        interface *p = addInterface(QString());
        p->read(stream);
    }
    stream >> myRouter;
    myRouteTable->read(stream);
    stream >> n;
    for ( i = 0 ; i < n; i++ ) {
        programm p(stream);
        installProgramm(p);
    }
}
/*!
  Задает устройству шлюз по умолчанию.
  @param str - строка с адресом.
*/
void smartDevice::setGateway(const QString &str)
{
    ipAddress t(str);
    routeRecord *i = myRouteTable->recordAt("0.0.0.0"); // Ищем старый шлюз
    if ( i ) myRouteTable->deleteFromTable(i); // Удаляем его
    ipAddress a = findInterfaceIp(t);
    if ( a.isEmpty() ) return;
    myRouteTable->addToTable(ipAddress(),ipAddress(),t,a,0,routeModel::staticMode);
}
//--------------------------------------------------------------

ipAddress smartDevice::gateway() const
{
    routeRecord *i = myRouteTable->recordAt("0.0.0.0");
    if ( i ) return i->gateway;
    return ipAddress();
}
/*!
  Отправляет сообщение посланное из интерфейса программы.
  @param a - Адрес назначения.
  @param size - Размер сообщения в кб(на деле сколько пакетов).
  @param type - Протокол с помощью которого происходит отправка.
*/
void smartDevice::sendMessage( const QString &a , int size ,int type)
{
    if ( type == TCP )  {
        tcpSocket *tcp = new tcpSocket(this,User);
        QByteArray temp(size*1024, '0');
        tcp->write(a,User,temp);
        connect(tcp, SIGNAL(writeEnd()), tcp, SLOT(deleteLater()));
        return;
    }
    udpSocket socket(this, User );
    QByteArray temp( size*1024 , '0');
    socket.write(a,User,temp);
}
//---------------------------------------------------------------
/*!
  Обрабатывает входящий пакет.
  @param p - указатель на пакет.
*/
void smartDevice::treatPacket(ipPacket &p)
{
    quint16 port = p.receiverSocket();
    foreach ( abstractSocket *i, mySockets )
        if ( i->isOurData(p.sender(),port) ) {
            i->treatPacket(p);            
            return;
        }
    if ( p.upProtocol() == UDP ) return;
    tcpSocket *tcp = new tcpSocket(this,User);
    tcp->treatPacket(p);
    connect(tcp, SIGNAL(receiveEnd()), tcp, SLOT(deleteLater()));
}
//--------------------------------------------------
/*!
  Узнает ip-адрес интерфейса лежащего в той же сети что и указанный адрес.
  @param a - Адрес для поиска.
  @return ip-адрес интерфейса.
*/
ipAddress smartDevice::findInterfaceIp(ipAddress a)
{
    foreach ( interface *i , myInterfaces ) {
        if ( !i->isConnect() ) continue;
        if ( (i->mask() & a ) == ( i->ip() & i->mask() ) ) return i->ip();
    }
    return ipAddress();
}
//------------------------------------------------------
/*!
  */
programm smartDevice::programmAt(const quint16 p) const
{
    foreach ( programm i , myProgramms )
        if ( i->id() == p ) return i;
    return programm();
}
//------------------------------------------------------
/*!
  Удаляет программу.
  @param p - указатель на программу.
*/
void smartDevice::removeProgramm(programm p)
{
    p->setDevice(NULL);
    myProgramms.removeOne(p);
}
//------------------------------------------------------
/*!
  Посылает всем программам установленым на компьютере прерывание.
  @param u - номер прерывания.
*/
bool smartDevice::sendInterrupt(int u)
{
    bool b = false;
    foreach ( programm i ,myProgramms )
        if ( i->isEnable() && i->interrupt(u) ) b = true;
    return b;
}
//-------------------------------------------------------
void smartDevice::tableDialog()
{
#ifndef __TESTING__
    routeEditor *d = new routeEditor(this);
    d->show();
#endif
}

void smartDevice::showLogDialog(logDialog *log) const
{
#ifndef __TESTING__
    connect( log ,SIGNAL(changeInterface(QString)) , this ,SLOT(setCheckedSocket(QString)) );
    foreach ( interface *i , myInterfaces ) {
        connect( i , SIGNAL(receiveData(frame,QString)) , log , SLOT(receiveData(frame,QString)) );
        connect( i , SIGNAL(sendData(frame,QString)) , log , SLOT(sendData(frame,QString)) );
    }
#endif
}

void smartDevice::adapterDialog()
{
#ifndef __TESTING__
    adapterProperty *d = new adapterProperty( new adapterSetting(this) );
    d->show();
#endif
}

void smartDevice::programmsDialog()
{
#ifndef __TESTING__
    programmDialog *d = new programmDialog;
    d->setDevice(this);
    d->exec();
#endif
}

void smartDevice::arpDialog()
{
#ifndef __TESTING__
    tableArp *d = new tableArp;
    d->setDevice(this);
    d->exec();
#endif
}

QStringList smartDevice::sockets() const
{
    QStringList t;
    for ( int i = 0 ; i < myInterfaces.size() ; i++ )
        t << myInterfaces.at(i)->name();
    return t;
}

void smartDevice::setCheckedSocket(const QString &str)
{
    foreach ( interface *i , myInterfaces )
        i->setChecked( i->name() == str );
}

QStringList smartDevice::interfacesIp() const
{
    QStringList t;
    for ( int  i = 0 ; i < myInterfaces.size() ; i++ )
        if ( myInterfaces.at(i)->isConnect() ) t << myInterfaces.at(i)->ip().toString();
    return t;
}

QString smartDevice::socketName(const cableDev *c) const
{
    for ( int i = 0 ; i < myInterfaces.size(); i++ )
        if ( myInterfaces.at(i)->isCableConnect(c) ) return myInterfaces.at(i)->name();
    return QString();
}

void smartDevice::deciSecondTimerEvent()
{
    foreach ( interface *i , myInterfaces )
        i->deciSecondEvent();
}

void smartDevice::secondTimerEvent()
{
    foreach ( interface *i , myInterfaces )
        i->secondEvent();
    foreach ( abstractSocket *i, mySockets )
        i->secondEvent();
    foreach ( programm i , myProgramms )
        i->incTime();
}

void smartDevice::setIp(const QString &a, const QString &ip)
{
    isDirty = true;
    adapter(a)->setIp(ip);
    connectedNet(adapter(a));
}

QString smartDevice::ipaddress(const QString &name) const
{
    return adapter(name)->ip().toString();
}

void smartDevice::setMask(const QString &a, const QString &ip)
{
    isDirty = true;
    adapter(a)->setMask(ip);
    connectedNet(adapter(a));
}

void smartDevice::checkReady() const
{
    myReady = true;
    foreach ( interface *i , myInterfaces )
        if ( i->isConnect() && i->ip().isEmpty() ) {
            myReady = false;
            break;
        }
}

bool smartDevice::isReady() const
{
    if ( isDirty ) {
        checkReady();
        isDirty = false;
    }
    return myReady;
}

bool smartDevice::isBusy() const
{
    foreach ( interface *i , myInterfaces )
        if ( i->isBusy() ) return true;
    foreach ( abstractSocket *i , mySockets )
        if ( i->isBusy() ) return true;
    return false;
}

void smartDevice::addInterface()
{
    int max = 0;
    foreach ( interface *i , myInterfaces ) {
        int t = i->name().mid(3).toInt();
        if ( t > max ) max = t;
    }
    addInterface( tr("eth%1").arg(++max) );
}

void smartDevice::deleteInterface(const QString &name)
{
    interface *t = adapter(name);
    myInterfaces.remove( myInterfaces.indexOf(t) );
    emit interfaceDeleted(name);
}

QList<arpModel*> smartDevice::arpModels()
{
    QList<arpModel*> list;
    foreach ( interface *i, myInterfaces )
        list += i->arpTable();
    return list;
}

void smartDevice::tableChanged(routeRecord *r,int n)
{
    r->change = routeModel::changed;
    sendInterrupt( n );
    r->change = routeModel::noChanged;
}

/*!
  * Устанавливает программу на устройство.
  * @param p - программа для установки.
  */
void smartDevice::installProgramm(programm p)
{
    p->setDevice(this);
    myProgramms << p;
}
//-----------------------------------------------------------

statistics smartDevice::deviceStatistics() const
{
    statistics s;
    foreach ( interface *i , myInterfaces )
        s += i->chipStatistics();
    return s;
}

bool smartDevice::hasProgramm(int id)
{
    foreach ( programm i , myProgramms )
        if ( i->id() == id ) return true;
    return false;
}

int smartDevice::trafficDigit() const
{
    int sum = 0;
    foreach ( interface *i , myInterfaces )
        sum += i->trafficDigit();
    return sum;
}

QIcon smartDevice::isConnectSocketIcon(const QString &socket) const
{
    if ( isConnectSocket(socket) ) return QIcon(":/im/images/ok.png");
    return QIcon(":/im/images/not.png");
}

//------------------------------------------------------------------------------
//----------------------Функии класса adapterSetting----------------------------
//------------------------------------------------------------------------------
/*!
  * Функция устанавливает текущий выбранный интерфейс в настройках адаптеров.
  */
void adapterSetting::setCurrent(int n)
{
    cur = n;
    oldMask = sd->myInterfaces[cur]->mask();
    oldIp = sd->myInterfaces[cur]->ip();
}
//-----------------------------------------------------------------------
/*!
  * Изменяет таблицу маршрутизации в соответсвии с новыми настройками интрефейсов.
  */
void adapterSetting::connectedNet()
{
    routeRecord *t = sd->routeTable()->recordAt( oldMask & oldIp );
    if ( t ) { // Удаляем запись со старыми натсройками
        sd->routeTable()->deleteFromTable(t);
    }
    sd->connectedNet(sd->myInterfaces.at(cur));
}
//--------------------------------------------------------------------------

bool adapterSetting::isUnderDhcpControl() const
{
    dhcpClientProgramm *t = qobject_cast<dhcpClientProgramm*>( sd->programmAt(programm::DHCPClient).impl() );
    return t->isUnderDhcpControl( sd->myInterfaces.at(cur)->name() );
}

void adapterSetting::setUnderDhcpControl(bool isUnder)
{
    dhcpClientProgramm *t = qobject_cast<dhcpClientProgramm*>( sd->programmAt( programm::DHCPClient).impl() );
    t->observeInterface( sd->myInterfaces.at(cur)->name() , isUnder );
}



