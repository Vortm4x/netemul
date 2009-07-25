#ifndef SWITCHCHIP_H
#define SWITCHCHIP_H

#include "boxchip.h"

class frame;
class switchModel;

class switchChip : public boxChip
{
public:
    switchChip(int c = 4);
    ~switchChip();
    void receiveEvent(frame *fr,devicePort *sender);
    void deleteFromTable(macAddress mac);
    void updateMac();
    void dialog();
    switchModel* model() { return switchTable; }
    void addToSwitchTable(const macAddress &m , const QString &p, int mode , int time);
private:
    switchModel *switchTable;
};

#endif // SWITCHCHIP_H
