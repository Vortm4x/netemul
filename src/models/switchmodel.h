#ifndef SWITCHMODEL_H
#define SWITCHMODEL_H

#include <QAbstractTableModel>
#include "macaddress.h"

class devicePort;

struct macRecord {
    macAddress mac;
    devicePort *port;
    int time;
    int mode;
    QString modeString() const {
        if ( mode == 0 ) return QObject::trUtf8("Статическая");
        else return QObject::trUtf8("Динамическая");
    }
};

class switchModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum { staticMode = 0 , dinamicMode = 1 };
    switchModel(QObject *parent = 0);
    int rowCount(const QModelIndex &p) const;
    int columnCount(const QModelIndex &p) const;
    Qt::ItemFlags flags(const QModelIndex &m) const;
    QVariant headerData(int s , Qt::Orientation o , int role) const;
    QVariant data(const QModelIndex &m, int role) const;
    bool removeRows( int row, int count, const QModelIndex & parent = QModelIndex() );

    void deleteFromTable(const macAddress &mac);
    macRecord* addToTable(const macAddress &mac ,devicePort *p , int mode , int time);
    void contains(const macAddress &m , devicePort *s);
    devicePort* portWithMac(const macAddress &m);
private:   
    QList<macRecord*> table;
};

#endif // SWITCHMODEL_H
