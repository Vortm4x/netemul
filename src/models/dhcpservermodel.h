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
#ifndef DHCPSERVERMODEL_H
#define DHCPSERVERMODEL_H

#include <QAbstractTableModel>
#include "macaddress.h"
#include "ipaddress.h"

class StaticDhcpRecordObject;

struct StaticDhcpRecord {
    MacAddress chaddr;
    IpAddress yiaddr;
    IpAddress mask;
    IpAddress gateway;
    int time;
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
};

static const int COLUMN_COUNT = 5;

class DhcpServerModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY( QVariantList staticDhcpRecordObject READ staticDhcpRecordObjectsList )
public:
    DhcpServerModel(QObject *parent = 0);
    ~DhcpServerModel();
    int rowCount( const QModelIndex &r = QModelIndex() ) const;
    int columnCount( const QModelIndex &r = QModelIndex() ) const;
    QVariant headerData( int s , Qt::Orientation o, int role ) const;
    Qt::ItemFlags flags(const QModelIndex &r) const;
    QVariant data(const QModelIndex &r, int role = Qt::DisplayRole ) const;
    bool insertRow(int, const QModelIndex &parent = QModelIndex() );
    bool removeRow(int row, const QModelIndex &parent = QModelIndex() );
    bool setData(const QModelIndex &index,const QVariant &value, int role = Qt::EditRole);

    Q_INVOKABLE void addStaticDhcpRecordObject(StaticDhcpRecordObject *r);
    QVariantList staticDhcpRecordObjectsList() const;

    bool containRecord( StaticDhcpRecord *rec) const;
    bool containRecord(IpAddress ip) const;
    StaticDhcpRecord* recordWithMac(MacAddress cha) const;
    void addStaticRecord(StaticDhcpRecord *rec);

    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
private:
    QList<StaticDhcpRecord*> myTable;
};

class StaticDhcpRecordObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY( QString chaddr READ chaddr WRITE setChaddr )
    Q_PROPERTY( QString yiaddr READ yiaddr WRITE setYiaddr )
    Q_PROPERTY( QString mask READ mask WRITE setMask )
    Q_PROPERTY( QString gateway READ gateway WRITE setGateway )
    Q_PROPERTY( int time READ time WRITE setTime )
public:
    StaticDhcpRecordObject(StaticDhcpRecord *record) {
        myRecord = record;
    }

    StaticDhcpRecordObject(QObject *parent = 0) : QObject(parent) {
        myRecord = new StaticDhcpRecord();
    }

    StaticDhcpRecord* record() const { return myRecord; }

    void setChaddr(const QString &str) { myRecord->chaddr.setMac(str); }
    void setYiaddr(const QString &str) { myRecord->yiaddr.setIp(str); }
    void setMask(const QString &str) { myRecord->mask.setIp(str); }
    void setGateway(const QString &str) { myRecord->gateway.setIp(str); }
    void setTime(int t) { myRecord->time = t; }
    QString chaddr() const { return myRecord->chaddr.toString(); }
    QString yiaddr() const { return myRecord->yiaddr.toString(); }
    QString mask() const { return myRecord->mask.toString(); }
    QString gateway() const { return myRecord->gateway.toString(); }
    int time() const { return myRecord->time; }
private:
    StaticDhcpRecord *myRecord;

};

#endif // DHCPSERVERMODEL_H
