#include "hubchip.h"
#include "deviceimpl.h"

hubChip::hubChip(int n /* = 4 */) : boxChip(n)
{

}

hubChip::~hubChip()
{
    qDeleteAll(mySockets);
    mySockets.clear();
}

void hubChip::receiveEvent(frame fr,devicePort *sender)
{
    foreach ( devicePort *i , mySockets )
        if ( i != sender && i->isConnect() )
            i->pushToSend(fr);
}

