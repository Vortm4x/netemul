#include "arpmodel.h"

arpModel::arpModel()
{
}

QVariant arpModel::data(const QModelIndex &i, int role /* = Qt::DisplayRole */ ) const
{
    if ( !i.isValid() || table.isEmpty() ) return QVariant();
    arpRecord *a = table.at(i.row());
    if ( role != Qt::DisplayRole ) return QVariant();
    switch ( i.column() ) {
        case 0: return a->mac.toString();
        case 1: return a->ip.toString();
        case 2: return a->time;
        case 3: return a->modeString();
    }
    return QVariant();
}

//QVariant arpModel::headerData(int s, Qt::Orientation o, int r /* = Qt::DisplayRole */) const
//{
//
//}
//
//int arpModel::rowCount(const QModelIndex &p/* = QModelIndex()*/) const
//{
//
//}
//    int columnCount(const QModelIndex &p = QModelIndex()) const;
//    Qt::ItemFlags flags(const QModelIndex &i) const;
