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

routeModel::routeModel(QObject *parent /* = 0 */) : QAbstractTableModel(parent)
{
    lastRecord = 0;
}

routeModel::~routeModel()
{
    qDeleteAll(table);
    table.clear();
}

int routeModel::rowCount(const QModelIndex &r) const
{
    if ( r.isValid() || table.isEmpty() ) return 0;
    return table.size();
}

int routeModel::columnCount( const QModelIndex&/* r = QModelIndex() */ ) const
{
    if ( table.isEmpty() ) return 0;
    return 6;
}

QVariant routeModel::headerData( int s , Qt::Orientation o, int role ) const
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

Qt::ItemFlags routeModel::flags( const QModelIndex &r) const
{
    if ( !r.isValid() || table.isEmpty() ) return 0;
    return QAbstractTableModel::flags(r);
}

QVariant routeModel::data(const QModelIndex &r, int role /* = Qt::DisplayRole */ ) const
{
    if ( !r.isValid() || table.isEmpty() ) return QVariant();
    routeRecord *rec = table.at(r.row());
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
routeRecord* routeModel::addToTable(IpAddress d,IpAddress m,IpAddress g,IpAddress o,qint8 metr,int mode)
{
    routeRecord *r = new routeRecord;
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
routeRecord* routeModel::addToTable(routeRecord *r)
{
    table << r;
    qStableSort(table.begin(),table.end(),routeGreat);    
    reset();
    emit recordAdding(r,addNet);
    return r;    
}
//------------------------------------------------------------
void routeModel::deleteFromTable(int n)
{
    int v = 0;
    foreach ( routeRecord *i , table )
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
void routeModel::deleteFromTable(routeRecord *r)
{
    emit recordDeleting(r,delNet);
    lastRecord = 0;
    table.removeOne(r);
    delete r;
    qStableSort(table.begin(),table.end(),routeGreat);
    reset();
}
//--------------------------------------------------------------
/*!
  Находит в таблице маршрутизации.
  @param a - адрес назначения.
  @return указатель на запись, если такой записи нет то NULL.
*/
routeRecord* routeModel::recordAt(const IpAddress &a) const
{
// Оптимизация работы, запоминаем полседний адрес и если при новом поиске
// он совпадает отправляем по той записи которая была последней =)
    if ( lastRecord && lastIpAddress == a ) return lastRecord;
    foreach ( routeRecord *i , table ) {
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

void routeModel::checkConnectedNet(IpAddress ip, IpAddress mask, bool add)
{
    IpAddress dest = mask & ip;
    foreach ( routeRecord *i , table )
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
QString routeRecord::modeString() const
{
    switch ( mode ) {
        case routeModel::staticMode : return QObject::tr("Static");
        case routeModel::ripMode : return QObject::tr("RIP");
        case routeModel::connectMode : return QObject::tr("Connected");
    }
    return QString();
}
//----------------------------------------------------------------

bool routeModel::isConnectedMode(QModelIndex curr)
{
    return (table.at(curr.row())->modeString() != tr("Connected"));
}

/*!
  * Находит в таблице измененую запись.
  * @return указатель на измененую запись, 0 - если такой записи нет.
  */
routeRecord* routeModel::changedRecord()
{
    foreach ( routeRecord *i , table )
        if ( i->change == changed ) return i;
    return 0;
}
//---------------------------------------------------------------------
/*!
  * Обновляет содержиое всех записей увеличивая время жизни.
  */
void routeModel::update()
{
    foreach ( routeRecord *i , table )
        if ( i->mode != connectMode && i->mode != staticMode ) i->time++;
}
//-----------------------------------------------------------------------
/*!
  * Удаляет из таблицы записи время жизни которых подошло к концу.
  * @param time - время жизни достигнув которого запись считается устаревшей.
  */
void routeModel::deleteOldRecord(int time)
{
    foreach ( routeRecord *i , table )
        if ( i->time >= time && i->mode != connectMode  && i->mode != staticMode ) deleteFromTable(i);
}

void routeModel::write(QDataStream &stream) const
{
    routeTable temp;
    foreach (routeRecord *i , table )
        if ( i->mode == routeModel::staticMode ) temp << i;
    stream << temp.size();
    foreach ( routeRecord *i , temp )
        stream << *i;
}

void routeModel::read(QDataStream &stream)
{
    table.clear();
    int n;
    stream >> n;
    for ( int i = 0 ; i < n ; i++ ) {
        routeRecord *t = new routeRecord;
        t->mode = staticMode;
        stream >> *t;
        addToTable(t);
    }
}


//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------




