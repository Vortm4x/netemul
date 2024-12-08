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
#include "routemodel.h"
#include <QStringBuilder>

#include <QtDebug>

RouteModel::RouteModel(QObject *parent /* = 0 */) : QAbstractTableModel(parent)
{
    lastRecord = 0;
}

RouteModel::~RouteModel()
{
    qDeleteAll(table);
    table.clear();
}

int RouteModel::rowCount(const QModelIndex &r) const
{
    if ( r.isValid() || table.isEmpty() ) return 0;
    return table.size();
}

int RouteModel::columnCount( const QModelIndex&/* r = QModelIndex() */ ) const
{
    if ( table.isEmpty() ) return 0;
    return 6;
}

QVariant RouteModel::headerData( int s , Qt::Orientation o, int role ) const
{
    if ( table.isEmpty() ) return QVariant();
    if ( o == Qt::Horizontal )
        if ( role == Qt::DisplayRole )
            switch (s) {
                case 0: return tr("Destination");
                case 1: return tr("Mask");
                case 2: return tr("Gateway");
                case 3: return tr("Interface");
                case 4: return tr("Metric");
                case 5: return tr("Source");
            }
    if ( o == Qt::Vertical )
        if ( role == Qt::DisplayRole ) return s+1;
    return QAbstractTableModel::headerData(s, o, role);
}

Qt::ItemFlags RouteModel::flags( const QModelIndex &r) const
{
    if ( !r.isValid() || table.isEmpty() ) return 0;
    return QAbstractTableModel::flags(r);
}

QVariant RouteModel::data(const QModelIndex &r, int role /* = Qt::DisplayRole */ ) const
{
    if ( !r.isValid() || table.isEmpty() ) return QVariant();
    RouteRecord *rec = table.at(r.row());
    if ( role == Qt::DisplayRole )
        switch ( r.column() ) {
            case 0: return rec->dest.toString();
            case 1: return rec->mask.toString();
            case 2: return rec->gateway.toString();
            case 3: return rec->out.toString();
            case 4: return rec->metric;
            case 5: return rec->modeString();
        }
    return QVariant();
}

/*!
  Добавляет в таблицу маршрутизации новую запись.
  @param d - сеть назначения.
  @param m - маска сети.
  @param g - адрес следующего маршрутизатора.
  @param o - интерфейс с которого отправляем.
  @param metr - метрика
  @param mode - источник записи.
  @return указатель на новую запись
*/
RouteRecord* RouteModel::addToTable(IpAddress d,IpAddress m,IpAddress g,IpAddress o,qint8 metr,int mode)
{
    RouteRecord *r = new RouteRecord;
    r->dest = d;
    r->mask = m;
    r->metric = metr;
    r->gateway = g;
    r->mode = mode;
    r->time = 0;
    r->change = noChanged;
    r->out = o;
    return addToTable(r);
}
//---------------------------------------------------------------
/*!
  Добавляет запись в таблицу маршрутизации.
  @param r - указатель на запись.
  @param tr - нужно ли вызывать прерывание(по умолчанию нужно).
  @return указатель добавленную на запись.
*/
RouteRecord* RouteModel::addToTable(RouteRecord *r)
{
    beginResetModel();
    table << r;
    qStableSort(table.begin(),table.end(),routeGreat);
    endResetModel();
    emit recordAdding(r,addNet);
    return r;    
}
//------------------------------------------------------------



void RouteModel::deleteFromTable(int n)
{
    int v = 0;
    foreach ( RouteRecord *i , table )
        if ( v++ == n ) {
            deleteFromTable(i);
            return;
        }
}
/*!
  Удаляет запись из таблицы маршрутизации.
  @param r - указатель на запись.
  @param tr - нужно ли вызывать прерывание(по умолчанию нужно).
*/
void RouteModel::deleteFromTable(RouteRecord *r)
{
    beginResetModel();
    emit recordDeleting(r,delNet);
    lastRecord = 0;
    table.removeOne(r);
    delete r;
    qStableSort(table.begin(),table.end(),routeGreat);
    endResetModel();
}
//--------------------------------------------------------------
/*!
  Находит в таблице маршрутизации.
  @param a - адрес назначения.
  @return указатель на запись, если такой записи нет то NULL.
*/
RouteRecord* RouteModel::recordAt(const IpAddress &a) const
{
// Оптимизация работы, запоминаем полседний адрес и если при новом поиске
// он совпадает отправляем по той записи которая была последней =)
    if ( lastRecord && lastIpAddress == a ) return lastRecord;
    foreach ( RouteRecord *i , table ) {
        if ( i->dest == ( a & i->mask ) ) {
            lastIpAddress = a;
            lastRecord = i;
            return i;
        }
    }
// Иначе 0
    return 0;
}
//---------------------------------------------

void RouteModel::checkConnectedNet(IpAddress ip, IpAddress mask, bool add)
{
    IpAddress dest = mask & ip;
    foreach ( RouteRecord *i , table )
        if ( i->dest == dest && i->mask == mask ) {
            if ( i->gateway == ip && add) return;
            deleteFromTable(i);
            if ( add ) break; else return;
        }
    addToTable( dest , mask , ip , ip , 0 , connectMode );
}

/*!
  @return строчка описывающая источник записи.
*/
QString RouteRecord::modeString() const
{
    switch ( mode ) {
        case RouteModel::staticMode : return QObject::tr("Static");
        case RouteModel::ripMode : return QObject::tr("RIP");
        case RouteModel::connectMode : return QObject::tr("Connected");
    }
    return QString();
}
//----------------------------------------------------------------

bool RouteModel::isConnectedMode(QModelIndex curr)
{
    return (table.at(curr.row())->modeString() != tr("Connected"));
}

/*!
  * Находит в таблице измененую запись.
  * @return указатель на измененую запись, 0 - если такой записи нет.
  */
RouteRecord* RouteModel::changedRecord()
{
    foreach ( RouteRecord *i , table )
        if ( i->change == changed ) return i;
    return 0;
}
//---------------------------------------------------------------------
/*!
  * Обновляет содержиое всех записей увеличивая время жизни.
  */
void RouteModel::update()
{
    foreach ( RouteRecord *i , table )
        if ( i->mode != connectMode && i->mode != staticMode ) i->time++;
}
//-----------------------------------------------------------------------
/*!
  * Удаляет из таблицы записи время жизни которых подошло к концу.
  * @param time - время жизни достигнув которого запись считается устаревшей.
  */
void RouteModel::deleteOldRecord(int time)
{
    foreach ( RouteRecord *i , table )
        if ( i->time >= time && i->mode != connectMode  && i->mode != staticMode ) deleteFromTable(i);
}

QVariantList RouteModel::recordObjectList() const
{
    QVariantList list;
    foreach ( RouteRecord *i , table ) {
        if ( i->mode == RouteModel::staticMode ) {
            QObject *o = new RouteRecordObject(i);
            list << qVariantFromValue(o);
        }
    }
    return list;
}

void RouteModel::addRouteRecordObject(RouteRecordObject *obj)
{
    addToTable( obj->record() );
    obj->deleteLater();
}

void RouteModel::write(QDataStream &stream) const
{
    RouteTable temp;
    foreach (RouteRecord *i , table )
        if ( i->mode == RouteModel::staticMode ) temp << i;
    stream << temp.size();
    foreach ( RouteRecord *i , temp )
        stream << *i;
}

void RouteModel::read(QDataStream &stream)
{
    table.clear();
    int n;
    stream >> n;
    for ( int i = 0 ; i < n ; i++ ) {
        RouteRecord *t = new RouteRecord;
        t->mode = staticMode;
        stream >> *t;
        addToTable(t);
    }
}


//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------




