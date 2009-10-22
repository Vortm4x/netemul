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

dhcpServerModel::~dhcpServerModel()
{
    qDebug("Nastya i'm destructor of dhcpServerModel, fix me please!!!");
}

int dhcpServerModel::rowCount( const QModelIndex &r/* = QModelIndex() */) const
{
    if ( r.isValid() || table.isEmpty() ) return 0;
    return table.size();
}

int dhcpServerModel::columnCount( const QModelIndex &r/* = QModelIndex() */) const
{
    if ( r.isValid() ) return 0;
    return COLUMN_COUNT;
}

QVariant dhcpServerModel::headerData( int s , Qt::Orientation o, int role ) const
{
    if ( role != Qt::DisplayRole ) return QVariant();
    if ( o == Qt::Horizontal )
        switch(s) {
            case 0: return tr("Mac-address");
            case 1: return tr("Ip-address");
            case 2: return tr("Mask");
            case 3: return tr("Gateway");
        }
    else return s+1;
    return QVariant();
}

Qt::ItemFlags dhcpServerModel::flags(const QModelIndex &r) const
{
    return QAbstractTableModel::flags(r);
}

QVariant dhcpServerModel::data(const QModelIndex &r, int role/* = Qt::DisplayRole */) const
{
    if ( !r.isValid() || table.isEmpty() ) return QVariant();
    staticDhcpRecord *rec = table.at( r.row() );
    if ( role == Qt::DisplayRole )
        switch( r.column() ) {
            case 0: return rec->chaddr.toString();
            case 1: return rec->yiaddr.toString();
            case 2: return rec->mask.toString();
            case 3: return rec->gateway.toString();
        }
    return QVariant();
}

bool dhcpServerModel::insertRow(int row,const QModelIndex &parent)
{
    Q_UNUSED(parent);
    staticDhcpRecord *newRecord = new staticDhcpRecord;
    newRecord->time = 0;
    table.insert(row,newRecord);
    reset();
    return true;
}

bool dhcpServerModel::removeRow(int row, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    staticDhcpRecord *record = table.takeAt(row);
    if ( record ) delete record;
    reset();
    return true;
}

bool dhcpServerModel::containRecord(staticDhcpRecord *rec)
{
    if ( table.isEmpty() ) return false;
    foreach ( staticDhcpRecord *i, table )
        if ( i == rec ) return true;
    return false;
}

staticDhcpRecord* dhcpServerModel::recordWithMac(macAddress cha)
{
    if ( table.isEmpty() ) return 0;
    foreach ( staticDhcpRecord *i, table )
        if ( i->chaddr == cha ) return i;
    return 0;
}

void dhcpServerModel::addStaticRecord(staticDhcpRecord *rec)
{
    table << rec;
}

void dhcpServerModel::write(QDataStream &stream) const
{
    stream << table.size();
    foreach ( staticDhcpRecord *i, table )
        i->write(stream);
}

void dhcpServerModel::read(QDataStream &stream)
{
    int s;
    stream >> s;
    for ( int i = 0; i < s; i++ ) {
        staticDhcpRecord *rec = new staticDhcpRecord;
        rec->read(stream);
        addStaticRecord(rec);
    }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void staticDhcpRecord::write(QDataStream &stream) const
{
    stream << chaddr << yiaddr << mask << gateway << time;
}

void staticDhcpRecord::read(QDataStream &stream)
{
    stream >> chaddr >> yiaddr >> mask >> gateway >> time;
}
