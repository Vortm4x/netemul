#ifndef HUBCHIP_H
#define HUBCHIP_H

#include "boxchip.h"
#include <QVector>


class hubChip : public boxChip
{
    Q_OBJECT
public:
    void receiveEvent(frame &fr,devicePort *sender);
    void addSocket(int n);
    bool isSharedBusy(cableDev *c) const;
    hubChip(int n = 4);
};

#endif // HUBCHIP_H
