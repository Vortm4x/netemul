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

arpModel::arpModel()
{
    lastRecord = 0;
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
    lastRecord = 0;
    emit tableChanged(t);
    return t;
}

void arpModel::deleteFromTable(const QString &ip)
{
    ipAddress a(ip);
    foreach ( arpRecord *i, myTable )
        if ( i->ip == a ) {
            deleteFromTable(i);
            return;
        }
}

void arpModel::deleteFromTable(arpRecord *r)
{
    myTable.removeOne(r);
    lastRecord = 0;
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
    if ( lastRecord && lastAddress == a ) return lastRecord;
    foreach ( arpRecord *i, myTable ) {
        if ( i->ip == a) {
            lastRecord = i;
            lastAddress = a;
            return i;
        }
    }
    return 0;
}

arpRecord* arpModel::recordAt(int u) const
{
    return myTable.at(u);
    return NULL;
}

int arpModel::size()
{
    return myTable.size();
}
