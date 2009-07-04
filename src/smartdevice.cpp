#include "smartdevice.h"
#include "programm.h"
#include <QtDebug>

smartDevice::smartDevice()
{
    myRouteMode = false;
}

smartDevice::~smartDevice()
{
    qDeleteAll(myRouteTable);
    myRouteTable.clear();
}

interface* smartDevice::adapter(QString s)
{
    foreach ( devicePort *i , mySockets )
        if ( i->name() == s ) return qobject_cast<interface*>(i->parentDev());
    return NULL;
}

routeRecord* smartDevice::addToTable(ipAddress dest,ipAddress mask,ipAddress gateway,ipAddress out,int time,qint8 metr,int mode)
{
    routeRecord *r = new routeRecord;
    r->out = NULL;
    r->dest = dest;
    r->mask = mask;
    r->metric = metr;
    r->gateway = gateway;
    r->mode = mode;
    r->time = time;
    if ( out != ipAddress("127.0.0.1") ) {
        r->out = ipToAdapter(out);
        if ( !r->out ) {delete r; return NULL; }
    }
    myRouteTable << r;
    qStableSort(myRouteTable.begin(),myRouteTable.end(),routeGreat);
    return r;
}

routeRecord* smartDevice::addToTable(routeRecord *r)
{
    myRouteTable << r;
    qStableSort(myRouteTable.begin(),myRouteTable.end(),routeGreat);
    return r;
}

void smartDevice::deleteFromTable(int n)
{
    int v = 0;
    foreach ( routeRecord *i , myRouteTable )
        if ( v++ == n ) {
            if ( i->mode == connectMode ) return;
            myRouteTable.removeOne(i);
            delete i;
            qStableSort(myRouteTable.begin(),myRouteTable.end(),routeGreat);
            return;
        }
}

void smartDevice::deleteFromTable(routeRecord *r)
{
    myRouteTable.removeOne(r);
    delete r;
    qStableSort(myRouteTable.begin(),myRouteTable.end(),routeGreat);
}

interface* smartDevice::ipToAdapter(ipAddress a)
{
    foreach ( devicePort *i , mySockets )
        if ( i->parentDev()->ip() == a ) return qobject_cast<interface*>(i->parentDev());
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
    ipAddress gw("0.0.0.0");
    if ( t->out->ip() != t->gateway ) gw = t->gateway;
    t->out->sendPacket(p,gw);
}
//---------------------------------------------
/*!
  Находит в таблице маршрутизации.
  @param a - адрес назначения.
  @return указатель на запись, если такой записи нет то NULL.
*/
routeRecord* smartDevice::recordAt(const ipAddress a) const
{
    foreach ( routeRecord *i , myRouteTable )
        if ( i->dest == ( a & i->mask ) ) return i;
    return NULL;
}
//---------------------------------------------
QString routeRecord::modeString() const
{
    switch ( mode ) {
        case smartDevice::staticMode : return trUtf8("Статическая");
        case smartDevice::ripMode : return trUtf8("RIP");
        case smartDevice::connectMode : return trUtf8("Подключена");
    }
    return QString();
}


void smartDevice::connectedNet(devicePort *p)
{
    bool add = true;
    if ( !p->isConnect() ) add = false;
    ipAddress o("0.0.0.0");
    ipAddress ip = p->parentDev()->ip();
    ipAddress mask = p->parentDev()->mask();
    if ( ip == o || mask == o ) return;
    ipAddress dest = mask & ip;
    foreach ( routeRecord *i , myRouteTable )
        if ( i->dest == dest && i->mask == mask ) {
            if ( i->gateway == ip && add) return;
            deleteFromTable(i);
            if ( add ) break ; else { myReady--; return; }
        }
    myReady++;
    addToTable( dest , mask , ip , ip , 0 , 0 , connectMode );
}

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

void smartDevice::write(QDataStream &stream) const
{
    stream << pos() << mySockets.count();
    foreach( devicePort *i, mySockets) {
        stream << *(i->parentDev());
        stream << *i;
    }
    int c = 0;
    foreach (routeRecord *i, myRouteTable) 
        if (i->mode == staticMode) c++;
    stream << c;
    foreach (routeRecord *i, myRouteTable) 
        if (i->mode == staticMode) stream << *i;
    stream << myRouteMode;
}

void smartDevice::read(QDataStream &stream)
{
    QPointF p;
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
    addToTable(ipAddress("0.0.0.0"),ipAddress("0.0.0.0"),t,a,0,0,staticMode);
}
//--------------------------------------------------------------
ipAddress smartDevice::gateway() const
{
    foreach ( routeRecord *i , myRouteTable )
        if ( i->mask.isEmpty() && i->dest.isEmpty() ) return i->gateway;
    return ipAddress("0.0.0.0");
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
    ipAddress gw("0.0.0.0");
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

QDataStream& operator<<(QDataStream &stream, const routeRecord &rec)
{
    stream << rec.dest;
    stream << rec.mask << rec.gateway;
    stream << rec.time << rec.metric << rec.out->ip();
    return stream;
}

QDataStream& operator>>(QDataStream &stream, routeRecord &rec)
{
    stream >> rec.dest >> rec.mask >> rec.gateway >> rec.time >> rec.metric;
    return stream;
}
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
    return ipAddress("0.0.0.0");
}
//------------------------------------------------------
