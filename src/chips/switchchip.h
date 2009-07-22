#ifndef SWITCHCHIP_H
#define SWITCHCHIP_H

#include "boxchip.h"
#include "frame.h"

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

class switchChip : public boxChip
{
public:
    enum { staticMode = 0 , dinamicMode = 1 };
    switchChip(int c = 4);
    void receiveEvent(frame fr,devicePort *sender);
    macRecord* addToTable(macAddress mac ,devicePort *p , int mode , int time);
    void deleteFromTable(macAddress mac);
    void updateMac();
private:
    QList<macRecord*> mySwitchTable;
};

#endif // SWITCHCHIP_H
