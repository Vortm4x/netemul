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
#ifndef SWITCHMODEL_H
#define SWITCHMODEL_H

#include <QAbstractTableModel>
#include "macaddress.h"

class DevicePort;

struct MacRecord {
    MacAddress mac;
    QString port;
    int time;
    int mode;
    QString modeString() const {
        if ( mode == 0 ) return QObject::tr("Static");
        else return QObject::tr("Dinamic");
    }
};

class MacRecordObject : public QObject {
    Q_OBJECT
    Q_PROPERTY( QString mac READ mac WRITE setMac )
    Q_PROPERTY( QString port READ port WRITE setPort )
    Q_PROPERTY( int time READ time WRITE setTime )
public:
    MacRecordObject(MacRecord *record) {
        r = record;
    }

    MacRecordObject(QObject *parent = 0) : QObject(parent) {
        r = new MacRecord();
        r->mode = 0;
    }

    MacRecord* record() const { return r; }

    QString mac() const { return r->mac.toString(); }
    void setMac(const QString &str) { r->mac.setMac(str); }
    QString port() const { return r->port; }
    void setPort(const QString &str) { r->port = str; }
    int time() const { return r->time; }
    void setTime(int t) { r->time = t; }

private:
    MacRecord *r;
};

typedef QList<MacRecord*> MacRecordList;

class SwitchModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY( QVariantList recordList READ recordList )
public:
    enum { staticMode = 0 , dinamicMode = 1 };
    SwitchModel(QObject *parent = 0);
    int rowCount(const QModelIndex &p) const;
    int columnCount(const QModelIndex &p) const;
    Qt::ItemFlags flags(const QModelIndex &m) const;
    QVariant headerData(int s , Qt::Orientation o , int role) const;
    QVariant data(const QModelIndex &m, int role) const;

    QVariantList recordList() const;

    void updateMac();
    void deleteFromTable(const MacAddress &mac);
    void deleteFromTable(MacRecord *r);
    MacRecord* addToTable(const MacAddress &mac ,const QString &port, int mode);
    void addToTable(MacRecord *rec);
    void contains(const MacAddress &m , const QString &port);
    QString portWithMac(const MacAddress &m);
private:   
    QString lastPort;
    MacAddress lastMac;
    MacRecordList table;
};

#endif // SWITCHMODEL_H
