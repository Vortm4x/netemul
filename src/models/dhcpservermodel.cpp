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
#include "dhcpservermodel.h"

DhcpServerModel::DhcpServerModel(QObject *parent /*= 0*/) : QAbstractTableModel(parent) {}

DhcpServerModel::~DhcpServerModel()
{
    qDeleteAll(myTable);
    myTable.clear();
}

int DhcpServerModel::rowCount( const QModelIndex &r/* = QModelIndex() */) const
{
    if ( r.isValid() || myTable.isEmpty() ) return 0;
    return myTable.size();
}

int DhcpServerModel::columnCount( const QModelIndex &r/* = QModelIndex() */) const
{
    if ( r.isValid() ) return 0;
    return COLUMN_COUNT;
}

QVariant DhcpServerModel::headerData( int s , Qt::Orientation o, int role ) const
{
    if ( role != Qt::DisplayRole ) return QVariant();
    if ( o == Qt::Horizontal )
        switch(s) {
            case 0: return tr("Mac-address");
            case 1: return tr("Ip-address");
            case 2: return tr("Mask");
            case 3: return tr("Gateway");
            case 4: return tr("Time");
        }
    else return s+1;
    return QVariant();
}

Qt::ItemFlags DhcpServerModel::flags(const QModelIndex &r) const
{
    if ( !r.isValid() ) return Qt::ItemIsEditable;
    return QAbstractTableModel::flags(r) | Qt::ItemIsEditable;
}

QVariant DhcpServerModel::data(const QModelIndex &r, int role/* = Qt::DisplayRole */) const
{
    if ( !r.isValid() || myTable.isEmpty() ) return QVariant();
    StaticDhcpRecord *rec = myTable.at( r.row() );
    if ( role == Qt::DisplayRole || role == Qt::EditRole )
        switch( r.column() ) {
            case 0: return rec->chaddr.toString();
            case 1: return rec->yiaddr.toString();
            case 2: return rec->mask.toString();
            case 3: return rec->gateway.toString();
            case 4: return rec->time;
        }
    return QVariant();
}

bool DhcpServerModel::setData(const QModelIndex &index, const QVariant &value, int role/* = Qt::EditRole*/)
{
    if ( index.isValid() && role == Qt::EditRole ) {
        StaticDhcpRecord *rec = myTable.at(index.row());
        if ( value.toString().isEmpty() ) return false;
        switch ( index.column() ) {
            case 0: rec->chaddr.setMac(value.toString()); break;
            case 1: rec->yiaddr.setIp(value.toString()); break;
            case 2: rec->mask.setIp(value.toString()); break;
            case 3: rec->gateway.setIp(value.toString()); break;
            case 4: rec->time = value.toInt(); break;
        }
        myTable.replace( index.row(), rec);
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

bool DhcpServerModel::insertRow(int,const QModelIndex &parent)
{
    Q_UNUSED(parent);
    StaticDhcpRecord *newRecord = new StaticDhcpRecord;
    newRecord->time = 0;
    myTable << newRecord;
    reset();
    return true;
}

bool DhcpServerModel::removeRow(int row, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    StaticDhcpRecord *record = myTable.takeAt(row);
    if ( record ) delete record;
    reset();
    return true;
}

bool DhcpServerModel::containRecord(StaticDhcpRecord *rec) const
{
    if ( myTable.isEmpty() ) return false;
    foreach ( StaticDhcpRecord *i, myTable )
        if ( i == rec ) return true;
    return false;
}

bool DhcpServerModel::containRecord(IpAddress ip) const
{
    if ( myTable.isEmpty() ) return false;
    foreach ( StaticDhcpRecord *i, myTable )
        if ( i->yiaddr == ip ) return true;
    return false;
}

StaticDhcpRecord* DhcpServerModel::recordWithMac(MacAddress cha) const
{
    if ( myTable.isEmpty() ) return 0;
    foreach ( StaticDhcpRecord *i, myTable )
        if ( i->chaddr == cha ) return i;
    return 0;
}

void DhcpServerModel::addStaticRecord(StaticDhcpRecord *rec)
{
    myTable << rec;
}

QVariantList DhcpServerModel::staticDhcpRecordObjectsList() const
{
    QVariantList list;
    foreach ( StaticDhcpRecord *i , myTable ) {
        QObject *o = new StaticDhcpRecordObject(i);
        list << qVariantFromValue(o);
    }
    return list;
}

void DhcpServerModel::addStaticDhcpRecordObject(StaticDhcpRecordObject *r)
{
    addStaticRecord( r->record() );
    r->deleteLater();
}

//-----------------------------------------------------------------------------

void DhcpServerModel::write(QDataStream &stream) const
{
    stream << myTable.size();
    foreach ( StaticDhcpRecord *i, myTable )
        i->write(stream);
}

void DhcpServerModel::read(QDataStream &stream)
{
    int s;
    stream >> s;
    for ( int i = 0; i < s; i++ ) {
        StaticDhcpRecord *rec = new StaticDhcpRecord;
        rec->read(stream);
        addStaticRecord(rec);
    }
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void StaticDhcpRecord::write(QDataStream &stream) const
{
    stream << chaddr << yiaddr << mask << gateway << time;
}

void StaticDhcpRecord::read(QDataStream &stream)
{
    stream >> chaddr >> yiaddr >> mask >> gateway >> time;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
