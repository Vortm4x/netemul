#ifndef ARPMODEL_H
#define ARPMODEL_H

#include "ipaddress.h"
#include "macaddress.h"

struct arpRecord;

class arpModel
{
public:
    enum { staticMode = 100 , dinamicMode = 101 };
    arpModel();
    ~arpModel() { clear(); }
    arpRecord* addToTable( ipAddress ip , macAddress mac , int mode );
    void deleteFromTable(const QString &ip);
    void deleteFromTable(arpRecord *r);
    void update();
    void clear();
    arpRecord* recordAt(const ipAddress &a) const;
    arpRecord* recordAt(int u) const;
    int size();
private:
    QList<arpRecord*> myTable;

};

struct arpRecord {
    macAddress mac;
    ipAddress ip;
    int time;
    int mode;
    QString modeString() const {
        if ( mode == arpModel::staticMode ) return QObject::trUtf8("Статическая");
        else return QObject::trUtf8("Динамическая");
    }
};

#endif // ARPMODEL_H
