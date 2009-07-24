#include "smartdevice.h"
#include "ripprogramm.h"
#include "routeeditor.h"
#include "adapterproperty.h"
#include "programmdialog.h"
#include "udppacket.h"
#include "tcppacket.h"
#include <QtDebug>

smartDevice::~smartDevice()
{
    qDeleteAll(myRouteTable);
    myRouteTable.clear();
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

/*!
  Добавляет в таблицу маршрутизации новую запись.
  @param d - сеть назначения.
  @param m - маска сети.
  @param g - адрес следующего маршрутизатора.
  @param o - интерфейс с которого отправляем.
  @param metr - метрика
  @param mode - источник записи.
  @return указатель на новую запись
*/
routeRecord* smartDevice::addToTable(ipAddress d,ipAddress m,ipAddress g,ipAddress o,qint8 metr,int mode)
{
    routeRecord *r = new routeRecord;
    r->dest = d;
    r->mask = m;
    r->metric = metr;
    r->gateway = g;
    r->mode = mode;
    r->time = 0;
    r->change = noChanged;
    r->out = o;
    return addToTable(r);
}
//---------------------------------------------------------------
/*!
  Добавляет запись в таблицу маршрутизации.
  @param r - указатель на запись.
  @param tr - нужно ли вызывать прерывание(по умолчанию нужно).
  @return указатель добавленную на запись.
*/
routeRecord* smartDevice::addToTable(routeRecord *r,bool tr /* = true */)
{
    myRouteTable << r;
    qStableSort(myRouteTable.begin(),myRouteTable.end(),routeGreat);
    r->change = changed;
    if ( tr ) sendInterrupt(addNet);
    return r;
}
//------------------------------------------------------------
void smartDevice::deleteFromTable(int n)
{
    int v = 0;
    foreach ( routeRecord *i , myRouteTable )
        if ( v++ == n ) {
            deleteFromTable(i);
            return;
        }
}
/*!
  Удаляет запись из таблицы маршрутизации.
  @param r - указатель на запись.
  @param tr - нужно ли вызывать прерывание(по умолчанию нужно).
*/
void smartDevice::deleteFromTable(routeRecord *r,bool tr /* = true*/)
{
    r->change = changed;
    if ( tr ) sendInterrupt(delNet);
    myRouteTable.removeOne(r);
    delete r;
    qStableSort(myRouteTable.begin(),myRouteTable.end(),routeGreat);
}
//--------------------------------------------------------------
interface* smartDevice::ipToAdapter(const ipAddress a)
{
    for ( int i = 0 ; i < myInterfaces.size() ; i++ )
        if ( myInterfaces.at(i)->ip() == a ) return myInterfaces[i];
    return NULL;
}

void smartDevice::receivePacket(ipPacket *p, interface *f)
{
    if ( p->receiver() == f->ip() || p->isBroadcast(f->mask()) ) treatPacket(p);
    else routePacket(p);
}
/*!
  Маршрутизирует пакет.
  @param p - указатель на пакет.
*/
void smartDevice::routePacket(ipPacket *p)
{
    if ( !myRouter ) return; // Выходим если нет маршрутизации.
    routeRecord *t = recordAt(p->receiver());
    if ( !t ) {
        delete p;
        return;
    }
    ipAddress gw;
    if ( t->out != t->gateway ) gw = t->gateway;
    ipToAdapter(t->out)->sendPacket(p,gw);
}
//---------------------------------------------
/*!
  Находит в таблице маршрутизации.
  @param a - адрес назначения.
  @return указатель на запись, если такой записи нет то NULL.
*/
routeRecord* smartDevice::recordAt(const ipAddress &a) const
{
    foreach ( routeRecord *i , myRouteTable )
        if ( i->dest == ( a & i->mask ) ) return i;
    return NULL;
}
//---------------------------------------------
/*!
  Находит запись в таблице с указанным портом.
  @param p - указатель на порт.
  @return указатель на запись или NULL если такой записи нет.
*/
routeRecord* smartDevice::recordAt(const interface *p)
{
    foreach ( routeRecord *i , myRouteTable )
        if ( ipToAdapter(i->out) == p ) return i;
    return 0;
}
//-------------------------------------------------------
/*!
  @return строчка описывающая источник записи.
*/
QString routeRecord::modeString() const
{
    switch ( mode ) {
        case smartDevice::staticMode : return QObject::trUtf8("Статическая");
        case smartDevice::ripMode : return QObject::trUtf8("RIP");
        case smartDevice::connectMode : return QObject::trUtf8("Подключена");
    }
    return QString();
}
//----------------------------------------------------------------
/*!
  Вызывается при подключении или отключении сети от устройства, а также
  при смене ip-адреса или маски подсети.
  @param p - порт на котором произошло событие;
*/
void smartDevice::connectedNet(interface *p)
{
    bool add = p->isConnect(); // Показывает происходит ли добавление.
    ipAddress ip = p->ip();
    ipAddress mask = p->mask();
    if ( ip.isEmpty() || mask.isEmpty() ) { // Если ip и маска пустые
        if ( routeRecord *t = recordAt(p) ) deleteFromTable(t);
        return;
    }
    ipAddress dest = mask & ip;
    foreach ( routeRecord *i , myRouteTable )
        if ( i->dest == dest && i->mask == mask ) {
            if ( i->gateway == ip && add) return;
            deleteFromTable(i);
            myReady--;
            if ( add ) break; else return;
        }
    myReady++;
    addToTable( dest , mask , ip , ip , 0 , connectMode );    
}
//------------------------------------------------------------

void smartDevice::addConnection(const QString &port,cableDev *c)
{
    adapter(port)->setConnect(true,c);
    connectedNet(adapter(port));
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
    foreach ( programm *i , myProgramms )  // И сами программы.
        stream << *i;
}
//-------------------------------------------------
void smartDevice::read(QDataStream &stream)
{
    deviceImpl::read(stream);
    int n,i;
    stream >> n;
    for (i = 0; i < n ; i++) {
        interface *p = addInterface(QString());
        p->read(stream);
    }
    stream >> myRouter;
    stream >> n;
    int t;
    programm *r;
    for ( i = 0; i < n; i++ ) {
        stream >> t;
        switch (t) {
            case RIP:
                r = new ripProgramm(this);
                break;
        }
        stream >> *r;
        installProgramm(r);
    }
}
/*!
  Задает устройству шлюз по умолчанию.
  @param str - строка с адресом.
*/
void smartDevice::setGateway(const QString &str)
{
    ipAddress t(str);
    foreach ( routeRecord *i , myRouteTable ) // Ищем старый шлюз
        if ( i->dest.isEmpty() && i->mask.isEmpty() ) {
            deleteFromTable(i); // Удаляем его
            break;
        }
    ipAddress a = findInterfaceIp(t);
    if ( a.isEmpty() ) return;
    addToTable(ipAddress(),ipAddress(),t,a,0,staticMode);
}
//--------------------------------------------------------------
ipAddress smartDevice::gateway() const
{
    foreach ( routeRecord *i , myRouteTable )
        if ( i->mask.isEmpty() && i->dest.isEmpty() ) return i->gateway;
    return ipAddress();
}
/*!
  Отправляет сообщение посланное из интерфейса программы.
  @param dest - Адрес назначения.
  @param size - Размер сообщения в кб(на деле сколько пакетов).
  @param pr - Протокол с помощью которого происходит отправка.
*/
void smartDevice::sendMessage( const QString &a , int size , int pr)
{
    Q_UNUSED(pr);
    ipAddress gw;
    routeRecord *r = recordAt(a);
    if ( !r ) return;
    if ( r->gateway != r->out ) gw = r->gateway;
    for ( int i = 0 ; i < size ; i++) {
        ipPacket *t = new ipPacket;
        t->setSender(r->out);
        t->setReceiver(a);
        ipToAdapter(r->out)->sendPacket(t,gw);
    }
}
//---------------------------------------------------------------
/*!
  Обновляет arp таблицу всех интерфейсов у данного устройства.
  @param u - максимальное время жизни.
*/
void smartDevice::updateArp()
{
    for ( int i = 0 ; i < myInterfaces.size() ; i++ )
        myInterfaces[i]->updateArp();
}
//---------------------------------------------------------------
/*!
  Обрабатывает входящий пакет.
  @param p - указатель на пакет.
*/
void smartDevice::treatPacket(ipPacket *p)
{
    int v;
    if ( p->upProtocol() == ipPacket::udp ) {
        udpPacket u(p->toData());
        v = u.receiver();
    }
    else {
        tcpPacket t(p->toData());
        v = t.receiver();
    }
    programm *t = programmAt( v );
    if ( t && t->isEnable() ) {
        t->execute(p);
        return;
    }
    delete p;
}
//--------------------------------------------------
/*!
  Ищет программу по номеру её порта.
  @param p - номер порта.
  @return Указатель на программу, либо NULL если такой программы нет.
*/
programm* smartDevice::programmAt(const quint16 p) const
{
    foreach ( programm *i , myProgramms )
        if ( i->socket() == p ) return i;
    return NULL;
}
//----------------------------------------------------
/*!
  Ищет программу по ее названию.
  @param n - название программы.
  @return Указатель на программу, либо NULL если такой программы нет.
*/
programm* smartDevice::programmAt(const QString n) const
{
    foreach ( programm *i , myProgramms )
        if ( i->name() == n ) return i;
    return NULL;
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
        if ( myInterfaces[i]->isConnect() ) continue;
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
void smartDevice::removeProgramm(programm *p)
{
    myProgramms.removeOne(p);
    delete p;
}
//------------------------------------------------------
/*!
  Посылает всем программам установленым на компьютере прерывание.
  @param u - номер прерывания.
*/
bool smartDevice::sendInterrupt(int u)
{
    bool b = false;
    foreach ( programm *i ,myProgramms )
        if ( i->isEnable() && i->interrupt(u) ) b = true;
    return b;
}
//-------------------------------------------------------
void smartDevice::tableDialog()
{
    SHOW_DIALOG(routeEditor)
}

void smartDevice::adapterDialog()
{
    adapterProperty *d = new adapterProperty;
    adapterSetting *set = new adapterSetting(this);
    d->setDevice(set);
    d->exec();
    delete set;
    delete d;
}

void smartDevice::programmsDialog()
{
    SHOW_DIALOG(programmDialog)
}

QStringList smartDevice::sockets() const
{
    QStringList t;
    for ( int i = 0 ; i < myInterfaces.size() ; i++ )
        t << myInterfaces.at(i)->name();
    return t;
}

QStringList smartDevice::interfacesIp() const
{
    QStringList t;
    for ( int  i = 0 ; i < myInterfaces.size() ; i++ )
        t << myInterfaces.at(i)->ip().toString();
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
    for ( int i = 0 ; i < myInterfaces.size() ; i++ )
        myInterfaces[i]->deciSecondEvent();
}

void smartDevice::setIp(const QString &a, const QString &ip)
{
    adapter(a)->setIp(ip);
    connectedNet(adapter(a));
}

void smartDevice::setMask(const QString &a, const QString &ip)
{
    adapter(a)->setMask(ip);
    connectedNet(adapter(a));
}



