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

class devicePort;

struct macRecord {
    macAddress mac;
    devicePort *port;
    int time;
    int mode;
    QString modeString() const {
        if ( mode == 0 ) return QObject::tr("Static");
        else return QObject::tr("Dinamic");
    }
};

class switchModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum { staticMode = 0 , dinamicMode = 1 };
    switchModel(QObject *parent = 0);
    int rowCount(const QModelIndex &p) const;
    int columnCount(const QModelIndex &p) const;
    Qt::ItemFlags flags(const QModelIndex &m) const;
    QVariant headerData(int s , Qt::Orientation o , int role) const;
    QVariant data(const QModelIndex &m, int role) const;

    void updateMac();
    void deleteFromTable(const macAddress &mac);
    void deleteFromTable(macRecord *r);
    macRecord* addToTable(const macAddress &mac ,devicePort *p , int mode);
    void contains(const macAddress &m , devicePort *s);
    devicePort* portWithMac(const macAddress &m);
private:   
    devicePort *lastPort;
    macAddress lastMac;
    QList<macRecord*> table;
};

#endif // SWITCHMODEL_H
