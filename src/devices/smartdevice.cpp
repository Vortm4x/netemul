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
#include <QMessageBox>
#include "smartdevice.h"
#ifndef __TESTING__
#include "routeeditor.h"
#include "adapterproperty.h"
#include "tablearp.h"
#include "programmdialog.h"
#include "logdialog.h"
#endif
#include "routemodel.h"
#include "abstractsocket.h"
#include "socketfactory.h"
#include "dhcpclientprogramm.h"


SmartDevice::SmartDevice(QObject *parent) : DeviceImpl(parent) , myRouter(false) ,
            myReady(false) , isDirty(true) , myRouteTable(0)
{
}

SmartDevice::~SmartDevice()
{    
    qDeleteAll(mySockets);
    mySockets.clear();
    qDeleteAll(myInterfaces);
    myInterfaces.clear();
}

const Interface* SmartDevice::adapter(const QString &s) const
{
    for ( int i = 0 ; i < myInterfaces.size() ; i++ )
        if ( myInterfaces.at(i)->name() == s ) return myInterfaces.at(i);
    return 0;
}

Interface* SmartDevice::adapter(const QString &name)
{
    for ( int i = 0 ; i < myInterfaces.size() ; i++ )
        if ( myInterfaces.at(i)->name() == name ) return myInterfaces[i];
    return 0;
}

void SmartDevice::addInterface(Interface *in)
{
    myInterfaces << in;
    connect( in, SIGNAL(receivedPacket(IpPacket)) , SLOT(receivePacket(IpPacket)) );
    connect( in , SIGNAL(cableConnected(Cable*)) , SLOT(addConnection(Cable*)) );
}

Interface* SmartDevice::addInterface(const QString &name)
{
    Interface *t = Interface::create(this);
    t->setName(name);
    addInterface( t );
    return t;
}

Interface* SmartDevice::ipToAdapter(const IpAddress &a)
{
    for ( int i = 0 ; i < myInterfaces.size() ; i++ )
        if ( myInterfaces.at(i)->ip() == a ) return myInterfaces[i];
    return NULL;
}

bool SmartDevice::hasConnentSockets() const {
    foreach ( Interface *i, myInterfaces ) {
        if ( i->isConnect() ) return true;
    }
    return false;
}

void SmartDevice::receivePacket(IpPacket p)
{
    if ( p.decTtl() == 0 ) return;
    Interface *f = qobject_cast<Interface*>(sender());
    if ( p.receiver() == f->ip() || p.isBroadcast(f->mask()) ) treatPacket(p);
    else routePacket(p);
}
/*!
  Маршрутизирует пакет.
  @param p - указатель на пакет.
*/
void SmartDevice::routePacket(IpPacket &p)
{
    if ( !myRouter ) return; // Выходим если нет маршрутизации.
    RouteRecord *t = myRouteTable->recordAt(p.receiver());
    if ( !t ) return;
    IpAddress gw;
    if ( t->out != t->gateway ) gw = t->gateway;
    Interface *f = ipToAdapter( t->out );
    if ( f && f->isConnect() ) {
        f->sendPacket(p,gw);
    } else {
        QMessageBox::warning(0, QObject::tr("The network is not working correctly"),
                                 QObject::tr("Can't route packet! See adapter settings!"),
                                 QMessageBox::Ok, QMessageBox::Ok);
    }
}
//---------------------------------------------
/*!
  Вызывается при подключении или отключении сети от устройства, а также
  при смене ip-адреса или маски подсети.
  @param p - порт на котором произошло событие;
*/
void SmartDevice::connectedNet(Interface *p)
{
    checkReady();    
    IpAddress ip = p->ip();
    IpAddress mask = p->mask();
    if ( ip.isEmpty() || mask.isEmpty() ) return;
    myRouteTable->checkConnectedNet(ip, mask, p->isConnect());
}
//------------------------------------------------------------

void SmartDevice::addConnection(Cable*)
{    
    Interface *i = qobject_cast<Interface*>(sender());
    connectedNet(i);
    emit interfaceConnected(i->name());
}

/*!
  Записывает устройство в поток данных.
  @param stream - ссылка на поток.
*/
void SmartDevice::write(QDataStream &stream) const
{
    DeviceImpl::write(stream);
    stream << myInterfaces.size(); // Количество сокетов
    for ( int i = 0 ; i < myInterfaces.size() ; i++ )
        myInterfaces[i]->write(stream);
    stream << myRouter; // Включена или нет маршрутизация.
    myRouteTable->write(stream);//
    stream << myPrograms.count(); // Количество программ.
    foreach ( Program *i , myPrograms )  // И сами программы.
        i->write(stream);
}
//-------------------------------------------------

void SmartDevice::read(QDataStream &stream)
{
    DeviceImpl::read(stream);
    int n,i;
    stream >> n;
    qDeleteAll(myInterfaces);
    myInterfaces.clear();
    for (i = 0; i < n ; i++) {
        Interface *p = addInterface(QString());
        p->read(stream);
    }
    stream >> myRouter;
    myRouteTable->read(stream);
    stream >> n;
    for ( i = 0 ; i < n; i++ ) {
        Program *p = Program::createFromStream(this,stream);
        addProgram(p);
    }
}

/*!
  Задает устройству шлюз по умолчанию.
  @param str - строка с адресом.
*/
void SmartDevice::setGateway(const QString &str)
{    
    if ( !myRouteTable ) {
        return;
    }
    IpAddress t(str);
    RouteRecord *i = myRouteTable->recordAt("0.0.0.0"); // Ищем старый шлюз
    if ( i ) myRouteTable->deleteFromTable(i); // Удаляем его
    if ( t.isEmpty() ) return;
    IpAddress a = findInterfaceIp(t);
    if ( a.isEmpty() ) {        
        return;
    }
    myRouteTable->addToTable(IpAddress(),IpAddress(),t,a,0,RouteModel::staticMode);
}
//--------------------------------------------------------------

IpAddress SmartDevice::gateway() const
{
    RouteRecord *i = myRouteTable->recordAt("0.0.0.0");
    if ( i ) return i->gateway;
    return IpAddress();
}
/*!
  Отправляет сообщение посланное из интерфейса программы.
  @param a - Адрес назначения.
  @param size - � азмер сообщения в кб(на деле сколько пакетов).
  @param type - Протокол с помощью которого происходит отправка.
*/
void SmartDevice::sendMessage( const QString &a , int size ,int type)
{
    AbstractSocket *s = openSocket( USER_PORT , type );
    s->setAutoDelete(true);
    QByteArray temp( size*1024 , '0');
    s->write(a,USER_PORT,temp);
}
//---------------------------------------------------------------
/*!
  Обрабатывает входящий пакет.
  @param p - указатель на пакет.
*/
void SmartDevice::treatPacket(IpPacket &p)
{
    quint16 port = p.receiverSocket();
    foreach ( AbstractSocket *i, mySockets )
        if ( i->isOurData(p.sender(),port) ) {
            i->treatPacket(p);            
            return;
        }
    if ( p.upProtocol() == UDP ) return;
    AbstractSocket *s = openSocket(USER_PORT , TCP);
    s->setAutoDelete(true);
    s->treatPacket(p);
}
//--------------------------------------------------
/*!
  Узнает ip-адрес интерфейса лежащего в той же сети что и указанный адрес.
  @param a - Адрес для поиска.
  @return ip-адрес интерфейса.
*/
IpAddress SmartDevice::findInterfaceIp(IpAddress a)
{
    foreach ( Interface *i , myInterfaces ) {
        if ( !i->isConnect() ) continue;
        if ( (i->mask() & a ) == ( i->ip() & i->mask() ) ) return i->ip();
    }
    return IpAddress();
}
//------------------------------------------------------
/*!
  */
Program* SmartDevice::programAt(const quint16 p) const
{
    foreach ( Program *i , myPrograms )
        if ( i->id() == p ) return i;
    return 0;
}
//------------------------------------------------------
/*!
  Удаляет программу.
  @param p - указатель на программу.
*/
void SmartDevice::removeProgram(Program *p)
{
    p->setDevice(NULL);
    myPrograms.removeOne(p);
    delete p;
}
//------------------------------------------------------
/*!
  Посылает всем программам установленым на компьютере прерывание.
  @param u - номер прерывания.
*/
bool SmartDevice::sendInterrupt(int u)
{
    bool b = false;
    foreach ( Program *i ,myPrograms )
        if ( i->isEnable() && i->interrupt(u) ) b = true;
    return b;
}
//-------------------------------------------------------
void SmartDevice::tableDialog()
{
#ifndef __TESTING__
    routeEditor *d = new routeEditor(this);
    d->show();
#endif
}

void SmartDevice::showLogDialog(logDialog *log) const
{
    connect( log ,SIGNAL(changeInterface(QString)) , this ,SLOT(setCheckedSocket(QString)) );
    foreach ( Interface *i , myInterfaces ) {
        connect( i , SIGNAL(receiveData(Frame,QString)) , log , SLOT(receiveData(Frame,QString)) );
        connect( i , SIGNAL(sendData(Frame,QString)) , log , SLOT(sendData(Frame,QString)) );
    }
}

void SmartDevice::adapterDialog()
{
    adapterProperty *d = new adapterProperty( new AdapterSetting(this) );
    d->show();
}

void SmartDevice::programmsDialog()
{
    programmDialog *d = new programmDialog;
    d->setDevice(this);
    d->show();
}

void SmartDevice::arpDialog()
{
    tableArp *d = new tableArp;
    d->setDevice(this);
    d->exec();
}

FeaturesMap SmartDevice::featuresList() const
{
    FeaturesMap t;
    foreach ( Program *i , myPrograms )
        t.insert(i->featureName(),i->isEnable());
    return t;
}

QStringList SmartDevice::sockets() const
{
    QStringList t;
    for ( int i = 0 ; i < myInterfaces.size() ; i++ )
        t << myInterfaces.at(i)->name();
    return t;
}

void SmartDevice::setCheckedSocket(const QString &str)
{
    foreach ( Interface *i , myInterfaces )
        i->setChecked( i->name() == str );
}

QList<Cable*> SmartDevice::cableList() const
{
    QList<Cable*> list;
    foreach ( Interface *i , myInterfaces ) {
        if ( i->isConnect() ) {
            list << i->socketCable();
        }
    }
    return list;
}

DevicePort* SmartDevice::findPortByName(const QString &name) const
{
    foreach ( Interface *i , myInterfaces ) {
        if ( i->name() == name ) {
            return i->socket();
        }
    }
    return 0;
}

QStringList SmartDevice::interfacesIp() const
{
    QStringList t;
    for ( int  i = 0 ; i < myInterfaces.size() ; i++ )
        if ( myInterfaces.at(i)->isConnect() ) t << myInterfaces.at(i)->ip().toString();
    return t;
}

QString SmartDevice::socketName(const Cable *c) const
{
    for ( int i = 0 ; i < myInterfaces.size(); i++ )
        if ( myInterfaces.at(i)->isCableConnect(c) ) return myInterfaces.at(i)->name();
    return QString();
}

void SmartDevice::deciSecondTimerEvent()
{
    foreach ( Interface *i , myInterfaces )
        i->deciSecondEvent();
}

void SmartDevice::secondTimerEvent()
{
    foreach ( Interface *i , myInterfaces )
        i->secondEvent();
    foreach ( AbstractSocket *i, mySockets )
        i->secondEvent();
    foreach ( Program *i , myPrograms )
        i->incTime();
}

void SmartDevice::setIp(const QString &a, const QString &ip)
{
    isDirty = true;
    adapter(a)->setIp(ip);
    connectedNet(adapter(a));
}

QString SmartDevice::ipaddress(const QString &name) const
{
    return adapter(name)->ip().toString();
}

void SmartDevice::setMask(const QString &a, const QString &ip)
{
    isDirty = true;
    adapter(a)->setMask(ip);
    connectedNet(adapter(a));
}

void SmartDevice::checkReady() const
{
    myReady = true;
    foreach ( Interface *i , myInterfaces )
        if ( i->isConnect() && i->ip().isEmpty() ) {
            myReady = false;
            break;
        }
}

bool SmartDevice::isReady() const
{
    if ( isDirty ) {
        checkReady();
        isDirty = false;
    }
    return myReady;
}

bool SmartDevice::isBusy() const
{
    foreach ( Interface *i , myInterfaces )
        if ( i->isBusy() ) return true;
    foreach ( AbstractSocket *i , mySockets )
        if ( i->isBusy() ) return true;
    return false;
}

bool SmartDevice::isConnect() const
{
    foreach ( Interface *i , myInterfaces ) {
        if ( i->isConnect() ) {
            return true;
        }
    }
    return false;
}

void SmartDevice::addInterface()
{
    int max = 0;
    foreach ( Interface *i , myInterfaces ) {
        int t = i->name().mid(3).toInt();
        if ( t > max ) max = t;
    }
    addInterface( tr("eth%1").arg(++max) );
}

void SmartDevice::deleteInterface(const QString &name)
{
    Interface *t = adapter(name);
    myInterfaces.remove( myInterfaces.indexOf(t) );
    emit interfaceDeleted(name);
}

QList<ArpModel*> SmartDevice::arpModels()
{
    QList<ArpModel*> list;
    foreach ( Interface *i, myInterfaces )
        list += i->arpTable();
    return list;
}

void SmartDevice::tableChanged(RouteRecord *r,int n)
{
    r->change = RouteModel::changed;
    sendInterrupt( n );
    r->change = RouteModel::noChanged;
}

/*!
  * Устанавливает программу на устройство.
  * @param p - программа для установки.
  */
void SmartDevice::addProgram(Program *p)
{
    p->setDevice(this);
    if ( p->parent() != this ) {
        p->setParent(this);
    }
    myPrograms << p;
    myView->onImplChange();
}
//-----------------------------------------------------------

Statistics SmartDevice::deviceStatistics() const
{
    Statistics s;
    foreach ( Interface *i , myInterfaces )
        s += i->statistics();
    return s;
}

bool SmartDevice::hasProgramm(int id)
{
    foreach ( Program *i , myPrograms )
        if ( i->id() == id ) return true;
    return false;
}

int SmartDevice::trafficDigit() const
{
    int sum = 0;
    foreach ( Interface *i , myInterfaces )
        sum += i->trafficDigit();
    return sum;
}

QIcon SmartDevice::isConnectSocketIcon(const QString &socket) const
{
    if ( isConnectSocket(socket) ) return QIcon(":/im/images/ok.png");
    return QIcon(":/im/images/not.png");
}

void SmartDevice::setRouteModel(RouteModel *model)
{
    if ( model != 0 ) {
        model->setParent(this);        
        connect( model , SIGNAL(recordAdding(RouteRecord*,int)) , SLOT(tableChanged(RouteRecord*,int)));
        connect( model , SIGNAL(recordDeleting(RouteRecord*,int)) , SLOT(tableChanged(RouteRecord*,int)));
    }
    myRouteTable = model;
}

AbstractSocket* SmartDevice::openSocket(quint16 port, int type)
{
    AbstractSocket *s = 0;
    if ( type == UDP ) {
        s = SocketFactory::getTempSocket(this,port,type);
    } else {
        s = SocketFactory::getTCPSocket(this,port);
    }
    connect( s , SIGNAL(imFinished(AbstractSocket*)) , this , SLOT(disposeSocket(AbstractSocket*)) );
    mySockets << s;
    return s;
}

void SmartDevice::disposeSocket(AbstractSocket *socket)
{
    if ( mySockets.contains(socket) ) {
        mySockets.removeOne(socket);
        delete socket;
    }
}

//------------------------------------------------------------------------------
//----------------------Функии класса AdapterSetting----------------------------
//------------------------------------------------------------------------------
/*!
  * Функция устанавливает текущий выбранный интерфейс в настройках адаптеров.
  */
void AdapterSetting::setCurrent(int n)
{
    cur = n;
    oldMask = sd->myInterfaces[cur]->mask();
    oldIp = sd->myInterfaces[cur]->ip();
}
//-----------------------------------------------------------------------
/*!
  * Изменяет таблицу маршрутизации в соответсвии с новыми настройками интрефейсов.
  */
void AdapterSetting::connectedNet()
{
    RouteRecord *t = sd->routeModel()->recordAt( oldMask & oldIp );
    if ( t ) { // Удаляем запись со старыми натсройками
        sd->routeModel()->deleteFromTable(t);
    }
    sd->connectedNet(sd->myInterfaces.at(cur));
}
//--------------------------------------------------------------------------

bool AdapterSetting::isUnderDhcpControl() const
{
    DhcpClientProgram *t = qobject_cast<DhcpClientProgram*>( sd->programAt(Program::DHCPClient) );
    return t->isUnderDhcpControl( sd->myInterfaces.at(cur)->name() );
}

void AdapterSetting::setUnderDhcpControl(bool isUnder)
{
    DhcpClientProgram *t = qobject_cast<DhcpClientProgram*>( sd->programAt( Program::DHCPClient) );
    t->observeInterface( sd->myInterfaces.at(cur)->name() , isUnder );
}




