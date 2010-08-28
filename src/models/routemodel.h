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
#include "scenexmlreader.h"
#include "scenexmlwriter.h"
#include "ipaddress.h"

struct routeRecord {
public:
    IpAddress dest;
    IpAddress mask;
    IpAddress gateway;
    IpAddress out;
    int time;
    qint8 metric;
    int mode;
    quint8 change; //!< Флаг показывающий изменена запись или нет.
    QString modeString() const;
    friend QDataStream& operator<<(QDataStream &stream, const routeRecord &rec);
    friend QDataStream& operator>>(QDataStream &stream, routeRecord &rec);
    void writeXml(sceneXmlWriter &stream) const;
    void readXml(SceneXmlReader &stream);
};

typedef QList<routeRecord*> routeTable;

class routeModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum { connectMode = 3 , staticMode = 4 , ripMode = 5 };
    enum { changed = 0 , noChanged = 1 };
    enum { addNet = 100 , delNet = 101 };
    routeModel(QObject *parent = 0);
    ~routeModel();
    int rowCount( const QModelIndex &r = QModelIndex() ) const;
    int columnCount( const QModelIndex &r = QModelIndex() ) const;
    QVariant headerData( int s , Qt::Orientation o, int role ) const;
    Qt::ItemFlags flags(const QModelIndex &r) const;
    QVariant data(const QModelIndex &r, int role = Qt::DisplayRole ) const;

    void update();
    void deleteOldRecord(int time);
    routeRecord* changedRecord();
    routeRecord* addToTable(IpAddress d,IpAddress m,IpAddress g,IpAddress o,qint8 metr,int mode);
    routeRecord* addToTable(routeRecord *r);
    void deleteFromTable(int n);
    void deleteFromTable(routeRecord *r);
    routeRecord* recordAt(const IpAddress &a) const;
    routeRecord* recordAt(int i) const { return table.at(i); }
    void checkConnectedNet(IpAddress ip, IpAddress mask, bool add);
    bool isConnectedMode(QModelIndex curr);
    void write(QDataStream &stream) const;
    void writeXml(sceneXmlWriter &stream) const;
    void readXml(SceneXmlReader &stream);
    void read(QDataStream &stream);
signals:
    void recordAdding(routeRecord*,int);
    void recordDeleting(routeRecord*,int);
private:
    mutable routeRecord *lastRecord;
    mutable IpAddress lastIpAddress;
    routeTable table;
};

inline QDataStream& operator<<(QDataStream &stream, const routeRecord &rec)
{
    stream << rec.dest;
    stream << rec.mask << rec.gateway;
    stream << rec.time << rec.metric << rec.out;
    return stream;
}
inline QDataStream& operator>>(QDataStream &stream, routeRecord &rec)
{
    stream >> rec.dest >> rec.mask >> rec.gateway >> rec.time >> rec.metric >> rec.out;
    return stream;
}

inline bool operator<(const routeRecord &e1 , const routeRecord &e2)
{
    if ( e1.mask != e2.mask ) return e1.mask < e2.mask;
    return e1.dest < e2.dest;
}

inline bool operator>(const routeRecord &e1 , const routeRecord &e2)
{
    if ( e1.mask != e2.mask ) return e1.mask > e2.mask;
    return e1.dest < e2.dest;
}

inline bool routeGreat(const routeRecord *e1 , const routeRecord *e2) { return *e1 > *e2; }

#endif // ROUTEMODEL_H
