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
#ifndef ROUTEMODEL_H
#define ROUTEMODEL_H

#include <QAbstractTableModel>
#include <QStringList>
#include "ipaddress.h"

struct RouteRecord {
public:
    static const int SAVE_SIZE = 6;
    IpAddress dest;
    IpAddress mask;
    IpAddress gateway;
    IpAddress out;
    int time;
    qint8 metric;
    int mode;
    quint8 change; //!< Флаг показывающий изменена запись или нет.
    QString modeString() const;
    friend QDataStream& operator<<(QDataStream &stream, const RouteRecord &rec);
    friend QDataStream& operator>>(QDataStream &stream, RouteRecord &rec);
};

class RouteRecordObject : public QObject {
    Q_OBJECT
    Q_PROPERTY( QString dest READ dest WRITE setDest )
    Q_PROPERTY( QString mask READ mask WRITE setMask )
    Q_PROPERTY( QString gateway READ gateway WRITE setGateway )
    Q_PROPERTY( QString out READ out WRITE setOut )
    Q_PROPERTY( int time READ time WRITE setTime )
    Q_PROPERTY( int metric READ metric WRITE setMetric )
public:
    RouteRecordObject(RouteRecord *r) { rec = r; }
    RouteRecordObject(QObject *parent = 0 ) : QObject(parent) {
        rec = new RouteRecord();
        rec->mode = 4;
    }

    RouteRecord* record() { return rec; }

    void setDest(const QString &str) { rec->dest.setIp(str); }
    QString dest() const { return rec->dest.toString(); }
    void setMask(const QString &str) { rec->mask.setIp(str); }
    QString mask() const { return rec->mask.toString(); }
    void setGateway(const QString &str) { rec->gateway.setIp(str); }
    QString gateway() const { return rec->gateway.toString(); }
    void setOut(const QString &str) { rec->out.setIp(str); }
    QString out() const { return rec->out.toString(); }
    int time() const { return rec->time; }
    void setTime(int t) { rec->time = t; }
    int metric() const { return rec->metric; }
    void setMetric(int t) { rec->metric = t; }

private:
    RouteRecord *rec;
};

typedef QList<RouteRecord*> RouteTable;

class RouteModel : public QAbstractTableModel
{
    Q_OBJECT    
    Q_PROPERTY( QVariantList recordList READ recordObjectList )
public:
    enum { connectMode = 3 , staticMode = 4 , ripMode = 5 };
    enum { changed = 0 , noChanged = 1 };
    enum { addNet = 100 , delNet = 101 };
    RouteModel(QObject *parent = 0);
    ~RouteModel();
    int rowCount( const QModelIndex &r = QModelIndex() ) const;
    int columnCount( const QModelIndex &r = QModelIndex() ) const;
    QVariant headerData( int s , Qt::Orientation o, int role ) const;
    Qt::ItemFlags flags(const QModelIndex &r) const;
    QVariant data(const QModelIndex &r, int role = Qt::DisplayRole ) const;

    void update();
    void deleteOldRecord(int time);
    RouteRecord* changedRecord();
    RouteRecord* addToTable(IpAddress d,IpAddress m,IpAddress g,IpAddress o,qint8 metr,int mode);
    RouteRecord* addToTable(RouteRecord *r);
    void deleteFromTable(int n);
    void deleteFromTable(RouteRecord *r);
    RouteRecord* recordAt(const IpAddress &a) const;
    RouteRecord* recordAt(int i) const { return table.at(i); }

    QVariantList recordObjectList() const;
    Q_INVOKABLE void addRouteRecordObject(RouteRecordObject *obj);

    void checkConnectedNet(IpAddress ip, IpAddress mask, bool add);
    bool isConnectedMode(QModelIndex curr);
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
signals:
    void recordAdding(RouteRecord*,int);
    void recordDeleting(RouteRecord*,int);
private:
    mutable RouteRecord *lastRecord;
    mutable IpAddress lastIpAddress;
    RouteTable table;
};

inline QDataStream& operator<<(QDataStream &stream, const RouteRecord &rec)
{
    stream << rec.dest;
    stream << rec.mask << rec.gateway;
    stream << rec.time << rec.metric << rec.out;
    return stream;
}
inline QDataStream& operator>>(QDataStream &stream, RouteRecord &rec)
{
    stream >> rec.dest >> rec.mask >> rec.gateway >> rec.time >> rec.metric >> rec.out;
    return stream;
}

inline bool operator<(const RouteRecord &e1 , const RouteRecord &e2)
{
    if ( e1.mask != e2.mask ) return e1.mask < e2.mask;
    return e1.dest < e2.dest;
}

inline bool operator>(const RouteRecord &e1 , const RouteRecord &e2)
{
    if ( e1.mask != e2.mask ) return e1.mask > e2.mask;
    return e1.dest < e2.dest;
}

inline bool routeGreat(const RouteRecord *e1 , const RouteRecord *e2) { return *e1 > *e2; }

#endif // ROUTEMODEL_H
