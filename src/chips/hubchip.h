#ifndef HUBCHIP_H
#define HUBCHIP_H

#include "boxchip.h"
#include <QVector>
#include "frame.h"

class hubChip : public boxChip
{
public:
    void receiveEvent(frame &fr,devicePort *sender);
    void addSocket(int n);
    bool isSharedBusy(cableDev *c) const;
    hubChip(int n = 4);
    ~hubChip();
};

#endif // HUBCHIP_H
