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
#include "tcpsocket.h"
#include "udpsocket.h"
#include "dhcpclientprogramm.h"


SmartDevice::SmartDevice(QObject *parent) : DeviceImpl(parent) , myRouter(false)
{
    myReady = false;
    isDirty = true;
    myRouteTable = new routeModel(this);
    connect( myRouteTable , SIGNAL(recordAdding(routeRecord*,int)) , SLOT(tableChanged(routeRecord*,int)));
    connect( myRouteTable , SIGNAL(recordDeleting(routeRecord*,int)) , SLOT(tableChanged(routeRecord*,int)));
}

SmartDevice::~SmartDevice()
{
    myProgramms.clear();
    qDeleteAll(mySockets);
    qDeleteAll(myInterfaces);
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

Interface* SmartDevice::addInterface(const QString &name)
{
    Interface *t = new Interface(this);
    t->setName(name);
    myInterfaces << t;
    connect( t, SIGNAL(receivedPacket(ipPacket)) , SLOT(receivePacket(ipPacket)) );
    return t;
}

Interface* SmartDevice::ipToAdapter(const IpAddress a)
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

void SmartDevice::receivePacket(ipPacket p)
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
void SmartDevice::routePacket(ipPacket &p)
{
    if ( !myRouter ) return; // Выходим если нет маршрутизации.
    routeRecord *t = myRouteTable->recordAt(p.receiver());
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

void SmartDevice::addConnection(const QString &port,cableDev *c)
{
    adapter(port)->setConnect(true,c);
    connectedNet(adapter(port));
    emit interfaceConnected(port);
}

void SmartDevice::deleteConnection(cableDev *c)
{
    foreach ( Interface *i , myInterfaces )
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
void SmartDevice::write(QDataStream &stream) const
{
    DeviceImpl::write(stream);
    stream << myInterfaces.size(); // Количество сокетов
    for ( int i = 0 ; i < myInterfaces.size() ; i++ )
        myInterfaces[i]->write(stream);
    stream << myRouter; // Включена или нет маршрутизация.
    myRouteTable->write(stream);//
    stream << myProgramms.count(); // Количество программ.
    foreach ( programm i , myProgramms )  // И сами программы.
        stream << i;
}
//-------------------------------------------------

void SmartDevice::writeXmlImpl(sceneXmlWriter &stream) const
{
    Q_UNUSED(stream)
//    deviceImpl::writeXml(stream);
//    stream.writeStartElement("smartdevice");
//    stream.writeAttribute("routing", ( myRouter ) ? "1" : "0" );
//    for ( int i = 0 ; i < myInterfaces.size() ; i++ ) {
//        stream.writeStartElement("chipimpl");
//        myInterfaces.at(i)->writeXml(stream);
//        stream.writeEndElement();
//    }
//    stream.writeStartElement("routetable");
//    myRouteTable->writeXml(stream);
//    stream.writeEndElement();
//    foreach ( programm i, myProgramms ) {
//        stream.writeStartElement("programm");
//        stream.writeAttribute( "id",QString::number(i->id()) );
//        i->writeXml(stream);
//        stream.writeEndElement();
//    }
//    stream.writeEndElement();
}

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
        programm p(stream);
        installProgramm(p);
    }
}

void SmartDevice::readXmlImpl(SceneXmlReader &stream)
{
    Q_UNUSED(stream);
//    Q_ASSERT( stream.isStartElement() && stream.name() == "impl" );
//    qDeleteAll(myInterfaces);
//    myInterfaces.clear();
//    while ( !stream.atEnd() ) {
//        stream.readNext();
//        if ( stream.isEndElement() ) break;
//        if ( stream.name() == "smartdevice" ) {
//            myRouter = stream.attributes().value("routing").toString().toInt();
//            while ( !stream.atEnd() ) {
//                stream.readNext();
//                if ( stream.isEndElement() ) break;
//                if ( stream.name() == "chipimpl" ) {
//                    interface *p = addInterface(QString());
//                    p->readXml(stream);
//                } else if (stream.name() == "routetable" ) {
//                    myRouteTable->readXml(stream);
//                }
//                else if ( stream.name() == "programm" ) {
//                    int id = stream.attributes().value("id").toString().toInt();
//                    programm p(id);
//                    p->readXml(stream);
//                    installProgramm(p);
//                }
//            }
//        } else if (stream.name() == "deviceimpl" )  {
//            deviceImpl::readXml(stream);
//        }
//    }
}

/*!
  Задает устройству шлюз по умолчанию.
  @param str - строка с адресом.
*/
void SmartDevice::setGateway(const QString &str)
{
    IpAddress t(str);
    routeRecord *i = myRouteTable->recordAt("0.0.0.0"); // Ищем старый шлюз
    if ( i ) myRouteTable->deleteFromTable(i); // Удаляем его
    if ( t.isEmpty() ) return;
    IpAddress a = findInterfaceIp(t);
    if ( a.isEmpty() ) {
        QMessageBox::warning(0, QObject::tr("The network is not working correctly"),
                                 QObject::tr("Can't set this gateway! See adapter settings!"),
                                 QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    myRouteTable->addToTable(IpAddress(),IpAddress(),t,a,0,routeModel::staticMode);
}
//--------------------------------------------------------------

IpAddress SmartDevice::gateway() const
{
    routeRecord *i = myRouteTable->recordAt("0.0.0.0");
    if ( i ) return i->gateway;
    return IpAddress();
}
/*!
  Отправляет сообщение посланное из интерфейса программы.
  @param a - Адрес назначения.
  @param size - Размер сообщения в кб(на деле сколько пакетов).
  @param type - Протокол с помощью которого происходит отправка.
*/
void SmartDevice::sendMessage( const QString &a , int size ,int type)
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
void SmartDevice::treatPacket(ipPacket &p)
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
programm SmartDevice::programmAt(const quint16 p) const
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
void SmartDevice::removeProgramm(programm p)
{
    p->setDevice(NULL);
    myProgramms.removeOne(p);
}
//------------------------------------------------------
/*!
  Посылает всем программам установленым на компьютере прерывание.
  @param u - номер прерывания.
*/
bool SmartDevice::sendInterrupt(int u)
{
    bool b = false;
    foreach ( programm i ,myProgramms )
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
#ifndef __TESTING__
    connect( log ,SIGNAL(changeInterface(QString)) , this ,SLOT(setCheckedSocket(QString)) );
    foreach ( Interface *i , myInterfaces ) {
        connect( i , SIGNAL(receiveData(frame,QString)) , log , SLOT(receiveData(frame,QString)) );
        connect( i , SIGNAL(sendData(frame,QString)) , log , SLOT(sendData(frame,QString)) );
    }
#endif
}

void SmartDevice::adapterDialog()
{
#ifndef __TESTING__
    adapterProperty *d = new adapterProperty( new adapterSetting(this) );
    d->show();
#endif
}

void SmartDevice::programmsDialog()
{
#ifndef __TESTING__
    programmDialog *d = new programmDialog;
    d->setDevice(this);
    d->show();
#endif
}

void SmartDevice::arpDialog()
{
#ifndef __TESTING__
    tableArp *d = new tableArp;
    d->setDevice(this);
    d->exec();
#endif
}

featuresMap SmartDevice::featuresList() const
{
    featuresMap t;
    foreach ( programm i , myProgramms )
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

QStringList SmartDevice::interfacesIp() const
{
    QStringList t;
    for ( int  i = 0 ; i < myInterfaces.size() ; i++ )
        if ( myInterfaces.at(i)->isConnect() ) t << myInterfaces.at(i)->ip().toString();
    return t;
}

QString SmartDevice::socketName(const cableDev *c) const
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
    foreach ( abstractSocket *i, mySockets )
        i->secondEvent();
    foreach ( programm i , myProgramms )
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
    foreach ( abstractSocket *i , mySockets )
        if ( i->isBusy() ) return true;
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

QList<arpModel*> SmartDevice::arpModels()
{
    QList<arpModel*> list;
    foreach ( Interface *i, myInterfaces )
        list += i->arpTable();
    return list;
}

void SmartDevice::tableChanged(routeRecord *r,int n)
{
    r->change = routeModel::changed;
    sendInterrupt( n );
    r->change = routeModel::noChanged;
}

/*!
  * Устанавливает программу на устройство.
  * @param p - программа для установки.
  */
void SmartDevice::installProgramm(programm p)
{
    p->setDevice(this);
    myProgramms << p;
    myView->onImplChange();
}
//-----------------------------------------------------------

statistics SmartDevice::deviceStatistics() const
{
    statistics s;
    foreach ( Interface *i , myInterfaces )
        s += i->chipStatistics();
    return s;
}

bool SmartDevice::hasProgramm(int id)
{
    foreach ( programm i , myProgramms )
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




