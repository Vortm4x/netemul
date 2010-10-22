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
#include "switchmodel.h"
#include "deviceport.h"
#include "appsetting.h"

SwitchModel::SwitchModel(QObject *parent /* = 0 */) : QAbstractTableModel(parent)
{
    lastPort = "";
}

int SwitchModel::rowCount(const QModelIndex &/* p */) const
{
    if ( table.isEmpty() ) return 0;
    return table.count();
}

int SwitchModel::columnCount(const QModelIndex &/* p */) const
{
    if ( table.isEmpty() ) return 0;
    return 4;
}

Qt::ItemFlags SwitchModel::flags(const QModelIndex &m) const
{
    return QAbstractTableModel::flags(m);
}

QVariant SwitchModel::headerData(int s , Qt::Orientation o , int role) const
{
    if ( role == Qt::TextAlignmentRole ) return Qt::AlignCenter;
    if ( role != Qt::DisplayRole ) return QVariant();
    if ( o == Qt::Horizontal ) {
        switch (s) {
            case 0: return tr("Mac-address");
            case 1: return tr("Port");
            case 2: return tr("Record type");
            case 3: return tr("TTL");
        }
    }
    return s+1;
}

QVariant SwitchModel::data(const QModelIndex &m, int role) const
{
    if ( !m.isValid() || table.isEmpty() ) return QVariant();
    if ( role == Qt::DisplayRole ) {
        MacRecord *t = table.at( m.row() );
        switch ( m.column() ) {
            case 0: return t->mac.toString();
            case 1: return t->port;
            case 2: return t->modeString();
            case 3: return t->time;
        }
    }
    return QVariant();
}

MacRecord* SwitchModel::addToTable(const macAddress &mac, const QString &port, int mode)
{
    foreach ( MacRecord *i , table )
        if ( i->mac == mac ) return NULL;
    MacRecord *t = new MacRecord;
    t->time = 0;
    t->mode = mode;
    t->port = port;
    t->mac = mac;
    addToTable(t);
    return t;
}

void SwitchModel::addToTable(MacRecord *rec)
{
    table << rec;
    reset();
}

void SwitchModel::deleteFromTable(const macAddress &mac)
{
    foreach ( MacRecord *i , table )
        if ( i->mac == mac ) {
            deleteFromTable(i);
            return;
        }
}

void SwitchModel::deleteFromTable(MacRecord *r)
{
    table.removeOne(r);
    delete r;
    lastPort = "";
    reset();
}

void SwitchModel::contains(const macAddress &m, const QString &port)
{
    bool cont = false;
    foreach ( MacRecord *i , table )
        if ( i->mac == m ) {
            if ( i->port != port && i->mode != staticMode ) i->port = port;
            cont = true;
            break;
        }
    if ( !cont ) addToTable( m , port , dinamicMode );
}

QString SwitchModel::portWithMac(const macAddress &m)
{
    if ( !lastPort.isEmpty() && m == lastMac ) return lastPort;
    foreach ( MacRecord *i , table ) {
        if ( i->mac == m ) {
            i->time = 0;
            lastMac = m;
            lastPort = i->port;
            return i->port;
        }
    }
    return "";
}

void SwitchModel::updateMac()
{
    int t = appSetting::ttlMac();
    foreach ( MacRecord *i , table )
        if ( ++i->time >= t ) deleteFromTable(i);
}

QVariantList SwitchModel::recordList() const
{
    QVariantList list;
    foreach ( MacRecord *i , table ) {
        if ( i->mode == staticMode ) {
            QObject *o = new MacRecordObject(i);
            list << qVariantFromValue(o);
        }
    }
    return list;
}
