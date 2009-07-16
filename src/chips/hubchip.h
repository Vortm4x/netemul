#ifndef HUBCHIP_H
#define HUBCHIP_H

#include "abstractchip.h"

class hubChip : public abstractChip
{
public:
    void receiveEvent(frame *fr,devicePort *sender);
    hubChip();
};

#endif // HUBCHIP_H
