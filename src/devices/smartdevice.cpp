#include <QtCore/QtDebug>
#include <QtGui/QMessageBox>
#include "smartdevice.h"
#ifndef __TESTING__
#include "routeeditor.h"
#include "routemodel.h"
#include "adapterproperty.h"
#include "tablearp.h"
#include "programmdialog.h"
#include "udppacket.h"
#include "tcppacket.h"


smartDevice::smartDevice() : myRouter(false)
{
    myReady = false;
    isDirty = true;
    myInterfaces.clear();
    myRouteTable = new routeModel(this);
    connect( myRouteTable , SIGNAL(recordAdding(routeRecord*,int)) , SLOT(tableChanged(routeRecord*,int)));
    connect( myRouteTable , SIGNAL(recordDeleting(routeRecord*,int)) , SLOT(tableChanged(routeRecord*,int)));
}

smartDevice::~smartDevice()
{
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
    return t;
}

interface* smartDevice::ipToAdapter(const ipAddress a)
{
    for ( int i = 0 ; i < myInterfaces.size() ; i++ )
        if ( myInterfaces.at(i)->ip() == a ) return myInterfaces[i];
    return NULL;
}

void smartDevice::receivePacket(ipPacket &p, interface *f)
{
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
  @param dest - Адрес назначения.
  @param size - Размер сообщения в кб(на деле сколько пакетов).
  @param pr - Протокол с помощью которого происходит отправка.
*/
void smartDevice::sendMessage( const QString &a , int size ,int)
{
    ipAddress gw;
    routeRecord *r = myRouteTable->recordAt(a);
    if ( !r ) return;
    if ( r->gateway != r->out ) gw = r->gateway;
    ipPacket t(r->out,a);
    for ( int i = 0 ; i < size ; i++)
        ipToAdapter(r->out)->sendPacket(t,gw);
}
//---------------------------------------------------------------
/*!
  Обрабатывает входящий пакет.
  @param p - указатель на пакет.
*/
void smartDevice::treatPacket(ipPacket &p)
{
    udpPacket u(p.unpack());
    int v = u.receiver();
    foreach ( programm i , myProgramms ) {
        if ( i->socket() == v && i->isEnable() ) {
            i->execute(p);
            break;
        }
    }
}
//--------------------------------------------------
/*!
  Ищет программу по номеру её порта.
  @param p - номер порта.
  @return Указатель на программу, либо NULL если такой программы нет.
*/
programm smartDevice::programmAt(const quint16 p) const
{
    foreach ( programm i , myProgramms )
        if ( i->socket() == p ) return i;
    return programm();
}
//----------------------------------------------------
/*!
  Ищет программу по ее названию.
  @param n - название программы.
  @return Указатель на программу, либо NULL если такой программы нет.
*/
programm smartDevice::programmAt(const QString n) const
{
    foreach ( programm i , myProgramms )
        if ( i->name() == n ) return i;
    return programm();
}
//----------------------------------------------------
/*!
  Узнает ip-адрес интерфейса лежащего в той же сети что и указанный адрес.
  @param a - Адрес для поиска.
  @return ip-адрес интерфейса.
*/
ipAddress smartDevice::findInterfaceIp(ipAddress a)
{
    for ( int i = 0 ; i < myInterfaces.size() ; ++i ) {
        if ( !myInterfaces[i]->isConnect() ) continue;
        if ( (myInterfaces[i]->ip() & myInterfaces[i]->mask() ) == ( a & myInterfaces[i]->mask() ) )
            return myInterfaces[i]->ip();
    }
    return ipAddress();
}
//------------------------------------------------------
/*!
  Удаляет программу.
  @param p - указатель на программу.
*/
void smartDevice::removeProgramm(programm p)
{
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
    routeEditor *d = new routeEditor(this);
    d->exec();
    delete d;
}

void smartDevice::adapterDialog()
{
    adapterSetting *set = new adapterSetting(this);
    adapterProperty *d = new adapterProperty(set);
    d->exec();
    delete set;
}

void smartDevice::programmsDialog()
{
    programmDialog *d = new programmDialog;
    d->setDevice(this);
    d->exec();
    delete d;
}

void smartDevice::arpDialog()
{
    tableArp *d = new tableArp;
    d->setDevice(this);
    d->exec();
    delete d;
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
        if ( i->isConnect() ) i->setChecked( i->name() == str );
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
    foreach ( interface *i , myInterfaces ) {
        if ( i->hasReceive() ) {
            ipPacket p = i->popFromReceive();
            receivePacket(p,i);
        }
        i->deciSecondEvent();
    }
}

void smartDevice::secondTimerEvent()
{
    foreach ( programm i , myProgramms )
        i->incTime();
    foreach ( interface *i , myInterfaces )
        i->secondEvent();
}

void smartDevice::setIp(const QString &a, const QString &ip)
{
    isDirty = true;
    adapter(a)->setIp(ip);
    connectedNet(adapter(a));
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
    foreach ( programm i , myProgramms )
        if ( i->name() == p->name() )  {
            QMessageBox::warning(0,tr("Error") , tr("Such a program already installed.") ,
                                 QMessageBox::Ok , QMessageBox::Ok );
            return;
        }
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

#endif



