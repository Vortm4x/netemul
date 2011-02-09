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
#ifndef ARPMODEL_H
#define ARPMODEL_H

#include "ipaddress.h"
#include "macaddress.h"

struct ArpRecord;
class ArpRecordObject;

class ArpModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY( QVariantList arpRecordObjectsList READ arpRecordObjectsList )
public:
    enum { staticMode = 100 , dinamicMode = 101 };
    ArpModel(QObject *parent = 0 );
    ~ArpModel() { clear(); }
    ArpRecord* addToTable( IpAddress ip , MacAddress mac , int mode );
    void deleteFromTable(const QString &ip);
    void deleteFromTable(ArpRecord *r);
    void update();
    void clear();
    ArpRecord* recordAt(const IpAddress &a) const;
    ArpRecord* recordAt(int u) const;
    int size();

    Q_INVOKABLE void addArpRecordObject(ArpRecordObject *r);
    QVariantList arpRecordObjectsList() const;

signals:
    void tableChanged(ArpRecord *record);

private:
    void addToTable(ArpRecord *rec);

private:
    QList<ArpRecord*> myTable;
    mutable ArpRecord *lastRecord;
    mutable IpAddress lastAddress;
};

struct ArpRecord {
    MacAddress mac;
    IpAddress ip;
    int time;
    int mode;
    QString modeString() const {
        if ( mode == ArpModel::staticMode ) return QObject::tr("Static");
        else return QObject::tr("Dinamic");
    }
};

class ArpRecordObject : public QObject {
    Q_OBJECT
    Q_PROPERTY( QString mac READ mac WRITE setMac )
    Q_PROPERTY( QString ip READ ip WRITE setIp )
    Q_PROPERTY( int time READ time WRITE setTime )
public:
    ArpRecordObject(QObject *parent = 0) : QObject(parent) {
        r = new ArpRecord();
        r->mode = ArpModel::staticMode;
    }
    ArpRecordObject(ArpRecord *rec) { r = rec; }
    ArpRecord* record() { return r; }

    void setTime(int t) { r->time = t; }
    void setMac(const QString &str) { r->mac.setMac(str); }
    void setIp(const QString &str) { r->ip.setIp(str); }
    QString mac() const { return r->mac.toString(); }
    QString ip() const { return r->ip.toString(); }
    int time() const { return r->time; }
private:
    ArpRecord *r;
};

#endif // ARPMODEL_H
