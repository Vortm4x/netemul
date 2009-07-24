#include "routemodel.h"

routeModel::routeModel(QObject *parent /* = 0 */) : QAbstractTableModel(parent)
{    
    head << trUtf8("Адрес назначения") << trUtf8("Маска") << trUtf8("Шлюз")
            << trUtf8("Интерфейс") << trUtf8("Метрика") << trUtf8("Источник");
}

int routeModel::rowCount(const QModelIndex &r) const
{
    if ( r.isValid() ||( table.count() == 0 ) ) return 0;
    return table.count();
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
        if ( role == Qt::DisplayRole ) return head.at(s);
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

