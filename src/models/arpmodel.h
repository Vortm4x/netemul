#ifndef ARPMODEL_H
#define ARPMODEL_H

#include <QAbstractTableModel>
#include "ipaddress.h"
#include "macaddress.h"

struct arpRecord {
    macAddress mac;
    ipAddress ip;
    int time;
    int mode;
    QString modeString() const {
        if ( mode ==  0 /* interface::staticMode */ ) return QObject::trUtf8("Статическая");
        else return QObject::trUtf8("Динамическая");
    }
};

class arpModel : public QAbstractTableModel
{
public:
    arpModel();
    QVariant data(const QModelIndex &i, int role = Qt::DisplayRole) const;
//    QVariant headerData(int s, Qt::Orientation o, int r = Qt::DisplayRole) const;
//    int rowCount(const QModelIndex &p = QModelIndex()) const;
//    int columnCount(const QModelIndex &p = QModelIndex()) const;
//    Qt::ItemFlags flags(const QModelIndex &i) const;
private:
    QList<arpRecord*> table;

};

#endif // ARPMODEL_H
