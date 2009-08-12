#ifndef SWITCHCHIP_H
#define SWITCHCHIP_H

#include "boxchip.h"

class switchModel;

class switchChip : public boxChip
{
    Q_OBJECT
public:
    switchChip(int c = 4);
    ~switchChip();
    void receiveEvent(frame &fr,devicePort *sender);
    void deleteFromTable(macAddress mac);
    void dialog();
    void secondTimerEvent();
    switchModel* model() { return switchTable; }
private:
    switchModel *switchTable;
};

#endif // SWITCHCHIP_H
