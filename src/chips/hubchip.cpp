#include "hubchip.h"
#include "deviceport.h"

hubChip::hubChip(int n /* = 4 */) : boxChip(n)
{
    foreach ( devicePort *i , mySockets )
        i->setShared(true);
}

hubChip::~hubChip()
{

}

void hubChip::receiveEvent(frame &fr,devicePort *sender)
{
    checkReceive(fr);
    foreach ( devicePort *i , mySockets )
        if ( i != sender && i->isConnect() ) {
            checkSender(fr);
            i->pushToSend( fr );
        }
}

void hubChip::addSocket(int n)
{
    boxChip::addSocket(n);
    mySockets[n-1]->setShared(true);
}

bool hubChip::isSharedBusy(cableDev *c) const
{
    foreach ( devicePort *i , mySockets ) {
        if ( !i->isCableConnect(c) && i->isCableBusy() ) return true;
    }
    return false;
}

