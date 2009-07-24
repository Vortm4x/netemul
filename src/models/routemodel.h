#ifndef ROUTEMODEL_H
#define ROUTEMODEL_H

#include <QAbstractTableModel>
#include "ipaddress.h"
#include <QStringList>

struct routeRecord {
public:
    ipAddress dest;
    ipAddress mask;
    ipAddress gateway;
    ipAddress out;
    int time;
    qint8 metric;
    int mode;
    quint8 change; //!< Флаг показывающий изменена запись или нет.
    QString modeString() const;
    friend QDataStream& operator<<(QDataStream &stream, const routeRecord &rec);
    friend QDataStream& operator>>(QDataStream &stream, routeRecord &rec);
};

class routeModel : public QAbstractTableModel
{
public:
    routeModel(QObject *parent = 0);
    int rowCount( const QModelIndex &r = QModelIndex()) const;
    int columnCount( const QModelIndex &r = QModelIndex() ) const;
    QVariant headerData( int s , Qt::Orientation o, int role ) const;
    Qt::ItemFlags flags(const QModelIndex &r) const;
    QVariant data(const QModelIndex &r, int role = Qt::DisplayRole ) const;
private:
    QList<routeRecord*> table;
    QStringList head;
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

#endif // ROUTEMODEL_H
