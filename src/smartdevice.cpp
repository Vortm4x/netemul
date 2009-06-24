#include "smartdevice.h"
#include "routeeditor.h"
#include "adapterproperty.h"
#include "tablearp.h"
#include <QtDebug>

smartDevice::smartDevice()
{
    myRouteMode = noRoute;
    time = 0;
}

smartDevice::~smartDevice()
{
    qDeleteAll(myRouteTable);
    myRouteTable.clear();
}

inline bool operator<(const routeRecord &e1 , const routeRecord &e2)
{
    if ( e1.mask != e2.mask )
        return e1.mask < e2.mask;
    return e1.dest < e2.dest;
}

inline bool operator>(const routeRecord &e1 , const routeRecord &e2)
{
    if ( e1.mask != e2.mask )
        return e1.mask > e2.mask;
    return e1.dest < e2.dest;
}

inline bool routeGreat(const routeRecord *e1 , const routeRecord *e2)
{
    return *e1 > *e2;
}

interface* smartDevice::adapter(QString s)
{
    foreach ( devicePort *i , mySockets )
        if ( i->name() == s ) return qobject_cast<interface*>(i->parentDev());
    return NULL;
}

void smartDevice::editorShow()
{
    routeEditor *d = new routeEditor;
    d->setDevice(this);
    d->exec();
    delete d;
}

void smartDevice::adapterShow()
{
    adapterProperty *d = new adapterProperty;
    d->setSmart(this);
    d->exec();
    delete d;
}

void smartDevice::arpShow()
{
    tableArp *d = new tableArp;
    d->setSmart(this);
    d->exec();
    delete d;
}

routeRecord* smartDevice::addToTable(ipAddress dest,ipAddress mask,ipAddress gateway,ipAddress out,int time,int metr,int mode)
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

void smartDevice::deleteFromTable(int n)
{
    int v = 0;
    foreach ( routeRecord *i , myRouteTable )
        if ( v++ == n ) {
            myRouteTable.removeOne(i);
            delete i;
            qStableSort(myRouteTable.begin(),myRouteTable.end(),routeGreat);
            return;
        }
}

void smartDevice::deleteFromTable(routeRecord *r)
{
    myRouteTable.removeOne(r);
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

void smartDevice::routePacket(ipPacket *p)
{
    if ( myRouteMode == noRoute ) return;
    ipAddress dest = p->receiver();
    foreach ( routeRecord *i ,myRouteTable )
        if ( i->dest == ( dest & i->mask )) {
            ipPacket *temp = new ipPacket;
            *temp = *p;
            if ( i->out->ip() != i->gateway ) i->out->sendPacket(temp,i->gateway);
            else i->out->sendPacket(temp);
            return;
        }
}

QString routeRecord::modeString() const
{
    switch ( mode ) {
        case smartDevice::staticMode : return QObject::trUtf8("Статическая");
        case smartDevice::ripMode : return QObject::trUtf8("RIP");
        case smartDevice::connectMode : return QObject::trUtf8("Подключена");
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
            if ( add ) break ; else return;
        }
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

void smartDevice::setGateway(const QString str)
{
    ipAddress t = ipAddress(str);
    foreach ( routeRecord *i , myRouteTable )
        if ( i->dest.isEmpty() && i->mask.isEmpty() ) {
            deleteFromTable(i);
            break;
        }
    foreach ( devicePort *i , mySockets ) {
        if ( (i->parentDev()->ip() & i->parentDev()->mask()) == ( t & i->parentDev()->mask() ) && i->isConnect() ) {
            addToTable(ipAddress("0.0.0.0"),ipAddress("0.0.0.0"),t,i->parentDev()->ip(),0,0,staticMode);
            break;
        }
    }
}

ipAddress smartDevice::gateway() const
{
    foreach ( routeRecord *i , myRouteTable )
        if ( i->mask.isEmpty() && i->dest.isEmpty() ) return i->gateway;
    return ipAddress("0.0.0.0");
}

void smartDevice::sendMessage(ipAddress dest , int size , int pr)
{
    Q_UNUSED(pr);
    ipAddress gw("0.0.0.0");
    interface *o = NULL;
    foreach ( routeRecord *i ,myRouteTable )
        if ( (i->mask & dest) == i->dest ) {
            if ( i->gateway == i->out->ip() ) gw = ipAddress("0.0.0.0");
            else gw = i->gateway;
            o = i->out;
            break;
        }
    if ( !o ) return;
    qDebug() << "Send " << size << " KB " << " from " << o->ip() << " to " << dest << " with gw= " << gw;
    for ( int i = 0 ; i < size ; i++) {
        ipPacket *t = new ipPacket;
        t->setSender(o->ip());
        t->setReceiver(dest);
        o->sendPacket(t,gw);
    }
}

void smartDevice::updateArp(int u)
{
    foreach ( devicePort *i , mySockets )
        i->parentDev()->updateArp(u);
}

void smartDevice::sendRip(int u)
{
    if ( ++time < u ||  myRouteMode != ripRoute ) return;
    qDebug() << "Router say HELLO to All !";
    foreach ( devicePort *i , mySockets )
        if ( i->isConnect() ) {
            ipPacket *p = new ipPacket;
            p->setSender( i->parentDev()->ip() );
            p->setBroadcast( i->parentDev()->mask() );
            *p << QVariant(QObject::tr("HELLO"));
            i->parentDev()->sendPacket(p);
        }
    time = 0;
}

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
