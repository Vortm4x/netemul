#include "smartdevice.h"
#include "ripprogramm.h"
#include <QtDebug>

smartDevice::~smartDevice()
{
    qDeleteAll(myRouteTable);
    myRouteTable.clear();
}

interface* smartDevice::adapter(QString s)
{
    foreach ( devicePort *i , mySockets )
        if ( i->name() == s ) return static_cast<interface*>(i->parentDev());
    return NULL;
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
    r->out = NULL;
    r->dest = d;
    r->mask = m;
    r->metric = metr;
    r->gateway = g;
    r->mode = mode;
    r->time = 0;
    r->change = noChanged;
    if ( o != ipAddress("127.0.0.1") ) {
        r->out = ipToAdapter(o);
        if ( !r->out ) {delete r; return NULL; }
    }
    return addToTable(r);
}
//---------------------------------------------------------------

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
    foreach ( devicePort *i , mySockets )
        if ( i->parentDev()->ip() == a ) return static_cast<interface*>(i->parentDev());
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
    if ( !myRouteMode ) return; // Выходим если нет маршрутизации.
    routeRecord *t = recordAt(p->receiver());
    if ( !t ) {
        delete p;
        return;
    }
    ipAddress gw;
    if ( t->out->ip() != t->gateway ) gw = t->gateway;
    t->out->sendPacket(p,gw);
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
        if ( i->out == p ) return i;
    return 0;
}
//-------------------------------------------------------
/*!
  @return строчка описывающая источник записи.
*/
QString routeRecord::modeString() const
{
    switch ( mode ) {
        case smartDevice::staticMode : return trUtf8("Статическая");
        case smartDevice::ripMode : return trUtf8("RIP");
        case smartDevice::connectMode : return trUtf8("Подключена");
    }
    return QString();
}
//----------------------------------------------------------------
/*!
  Вызывается при подключении или отключении сети от устройства, а также
  при смене ip-адреса или маски подсети.
  @param p - порт на котором произошло событие;
*/
void smartDevice::connectedNet(devicePort *p)
{
    bool add = p->isConnect(); // Показывает происходит ли добавление.
    ipAddress ip = p->parentDev()->ip();
    ipAddress mask = p->parentDev()->mask();
    if ( ip.isEmpty() || mask.isEmpty() ) { // Если ip и маска пустые
        if ( routeRecord *t = recordAt(static_cast<interface*>(p->parentDev()) )) deleteFromTable(t);
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
void smartDevice::addConnection(cableDev *cable)
{
    device::addConnection(cable);
    if ( cable->start() == this ) connectedNet(cable->startPort() );
    else connectedNet(cable->endPort() );
}

void smartDevice::deleteConnection(cableDev *cable)
{
    if ( cable->start() == this ) connectedNet(cable->startPort() );
    else connectedNet(cable->endPort() );
    device::deleteConnection(cable);
}
/*!
  Записывает устройство в поток данных.
  @param stream - ссылка на поток.
*/
void smartDevice::write(QDataStream &stream) const
{
    stream << pos() << mySockets.count(); // Количество сокетов
    foreach( devicePort *i, mySockets) {
        stream << *(i->parentDev()); // Их адаптеры
        stream << *i;
    }
    int c = 0; // Количество статических записей в таблице маршрутизации.
    foreach (routeRecord *i, myRouteTable) 
        if (i->mode == staticMode) c++;
    stream << c;
    foreach (routeRecord *i, myRouteTable) 
        if (i->mode == staticMode) stream << *i;
    stream << myRouteMode; // Включена или нет маршрутизация.
    stream << myProgramms.count(); // Количество программ.
    foreach ( programm *i , myProgramms )  // И сами программы.
        stream << *i;
    stream << toolTip();
}
//-------------------------------------------------
void smartDevice::read(QDataStream &stream)
{
    QPointF p;
    QString str;
    int n,i;
    stream >> p >> n;
    setPos(p);
    foreach ( devicePort *i , mySockets )
        removeSocket(i);
    for (i = 0; i < n ; i++) {
        devicePort *p = addInterface(QString(),0);
        stream >> *(p->parentDev());
        stream >> *p;
    }
    stream >> n;
    for ( i = 0; i < n ; i++) {
        routeRecord *rec = new routeRecord;
        stream >> *rec;
        ipAddress ip;
        stream >> ip;
        rec->out = ipToAdapter(ip);
        rec->mode = staticMode;
        myRouteTable.append(rec);
    }
    stream >> myRouteMode;
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
    stream >> str;
    setToolTip(str);
}
/*!
  Задает устройству шлюз по умолчанию.
  @param str - строка с адресом.
*/
void smartDevice::setGateway(const QString str)
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
void smartDevice::sendMessage(ipAddress dest , int size , int pr)
{
    Q_UNUSED(pr);
    ipAddress gw;
    routeRecord *r = recordAt(dest);
    if ( !r ) return;
    if ( r->gateway != r->out->ip() ) gw = r->gateway;
    for ( int i = 0 ; i < size ; i++) {
        ipPacket *t = new ipPacket;
        t->setSender(r->out->ip());
        t->setReceiver(dest);
        r->out->sendPacket(t,gw);
    }
}
//---------------------------------------------------------------
/*!
  Обновляет arp таблицу всех интерфейсов у данного устройства.
  @param u - максимальное время жизни.
*/
void smartDevice::updateArp(int u)
{
    foreach ( devicePort *i , mySockets )
        i->parentDev()->updateArp(u);
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
        udpPacket u;
        *p >> u;
        v = u.receiver();
    }
    else {
        tcpPacket t;
        *p >> t;
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
  Вызывает тик таймера у всех работающих на устройстве программ.
*/
void smartDevice::incTime()
{
    foreach ( programm *i , myProgramms )
        if ( i->isEnable() ) i->incTime();
}
//------------------------------------------------------
/*!
  Узнает ip-адрес интерфейса лежащего в той же сети что и указанный адрес.
  @param a - Адрес для поиска.
  @return ip-адрес интерфейса.
*/
ipAddress smartDevice::findInterfaceIp(ipAddress a)
{
    foreach ( devicePort *i , mySockets ) {
        if ( !i->isConnect() ) continue;
        if ( (i->parentDev()->ip() & i->parentDev()->mask() ) == ( a & i->parentDev()->mask() ) )
            return i->parentDev()->ip();
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

