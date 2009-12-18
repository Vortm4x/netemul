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

struct arpRecord;

class arpModel : public QObject
{
    Q_OBJECT
public:
    enum { staticMode = 100 , dinamicMode = 101 };
    arpModel();
    ~arpModel() { clear(); }
    arpRecord* addToTable( ipAddress ip , macAddress mac , int mode );
    void deleteFromTable(const QString &ip);
    void deleteFromTable(arpRecord *r);
    void update();
    void clear();
    arpRecord* recordAt(const ipAddress &a) const;
    arpRecord* recordAt(int u) const;
    int size();
signals:
    void tableChanged(arpRecord *record);
private:
    QList<arpRecord*> myTable;
    mutable arpRecord *lastRecord;
    mutable ipAddress lastAddress;
};

struct arpRecord {
    macAddress mac;
    ipAddress ip;
    int time;
    int mode;
    QString modeString() const {
        if ( mode == arpModel::staticMode ) return QObject::tr("Static");
        else return QObject::tr("Dinamic");
    }
};

#endif // ARPMODEL_H
