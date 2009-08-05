#include "arpmodel.h"
#include "appsetting.h"

arpModel::arpModel()
{
}

arpRecord* arpModel::addToTable(ipAddress ip , macAddress mac , int mode )
{
    foreach ( arpRecord *i , myTable ) {
        if ( i->ip == ip && i->mac == mac ) return i;
        if ( i->mode != staticMode && (i->ip == ip || i->mac == mac ) ) {
            i->ip = ip;
            i->mac = mac;
            i->mode = mode;
            return i;
        }
    }
    arpRecord *t = new arpRecord;
    t->ip = ip;
    t->mac = mac;
    t->mode = mode;
    t->time = 0;
    myTable << t;
    return t;
}

void arpModel::deleteFromTable(const QString &ip)
{
    ipAddress a(ip);
    foreach ( arpRecord *i, myTable )
        if ( i->ip == a ) deleteFromTable(i);
}

void arpModel::deleteFromTable(arpRecord *r)
{
    myTable.removeOne(r);
    delete r;
}

void arpModel::update()
{
    int n = appSetting::ttlArp();
    foreach ( arpRecord *i , myTable )
        if ( ++i->time >= n ) deleteFromTable(i);
}

void arpModel::clear()
{
    qDeleteAll(myTable);
    myTable.clear();
}

arpRecord* arpModel::recordAt(const ipAddress &a) const
{
    foreach ( arpRecord *i, myTable )
        if ( i->ip == a) return i;
    return NULL;
}

