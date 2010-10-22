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
#include "arpmodel.h"
#include "appsetting.h"

ArpModel::ArpModel(QObject *parent) : QObject(parent)
{
    lastRecord = 0;
}

ArpRecord* ArpModel::addToTable(IpAddress ip , macAddress mac , int mode )
{
    foreach ( ArpRecord *i , myTable ) {
        if ( i->ip == ip && i->mac == mac ) return i;
        if ( i->mode != staticMode && (i->ip == ip || i->mac == mac ) ) {
            i->ip = ip;
            i->mac = mac;
            i->mode = mode;
            return i;
        }
    }
    ArpRecord *t = new ArpRecord;
    t->ip = ip;
    t->mac = mac;
    t->mode = mode;
    t->time = 0;
    addToTable(t);
    emit tableChanged(t);
    return t;
}

void ArpModel::addToTable(ArpRecord *rec)
{
    lastRecord = 0;
    myTable << rec;
}

void ArpModel::deleteFromTable(const QString &ip)
{
    IpAddress a(ip);
    foreach ( ArpRecord *i, myTable )
        if ( i->ip == a ) {
            deleteFromTable(i);
            return;
        }
}

void ArpModel::deleteFromTable(ArpRecord *r)
{
    myTable.removeOne(r);
    lastRecord = 0;
    delete r;
}

void ArpModel::update()
{
    int n = appSetting::ttlArp();
    foreach ( ArpRecord *i , myTable )
        if ( ++i->time >= n ) deleteFromTable(i);
}

void ArpModel::clear()
{
    qDeleteAll(myTable);
    myTable.clear();
}

ArpRecord* ArpModel::recordAt(const IpAddress &a) const
{
    if ( lastRecord && lastAddress == a ) return lastRecord;
    foreach ( ArpRecord *i, myTable ) {
        if ( i->ip == a) {
            lastRecord = i;
            lastAddress = a;
            return i;
        }
    }
    return 0;
}

ArpRecord* ArpModel::recordAt(int u) const
{
    return myTable.at(u);
    return NULL;
}

int ArpModel::size()
{
    return myTable.size();
}

QVariantList ArpModel::arpRecordObjectsList() const
{
    QVariantList list;
    foreach ( ArpRecord *i , myTable ) {
        if ( i->mode == ArpModel::staticMode ) {
            QObject *o = new ArpRecordObject(i);
            list << qVariantFromValue(o);
        }
    }
    return list;
}

void ArpModel::addArpRecordObject(ArpRecordObject *r)
{
    addToTable( r->record() );
    r->deleteLater();
}
