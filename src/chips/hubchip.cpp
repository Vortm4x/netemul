#include "hubchip.h"
#include "deviceport.h"

hubChip::hubChip(int n /* = 4 */) : boxChip(n)
{

}

hubChip::~hubChip()
{
    qDeleteAll(mySockets);
    mySockets.clear();
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
