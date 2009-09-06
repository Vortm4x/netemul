#ifndef ROUTEMODEL_H
#define ROUTEMODEL_H

#include <QAbstractTableModel>
#include <QStringList>
#include "ipaddress.h"

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
    routeRecord* addToTable(ipAddress d,ipAddress m,ipAddress g,ipAddress o,qint8 metr,int mode);
    routeRecord* addToTable(routeRecord *r);
    void deleteFromTable(int n);
    void deleteFromTable(routeRecord *r);
    routeRecord* recordAt(const ipAddress &a) const;
    routeRecord* recordAt(int i) const { return table.at(i); }
    void checkConnectedNet(ipAddress ip, ipAddress mask, bool add);
    bool isConnectedMode(QModelIndex curr);
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
signals:
    void recordAdding(routeRecord*,int);
    void recordDeleting(routeRecord*,int);
private:
    mutable routeRecord *lastRecord;
    mutable ipAddress lastIpAddress;
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
