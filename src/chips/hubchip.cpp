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

void hubChip::receiveEvent(frame *fr,devicePort *sender)
{
#ifndef __TESTING__
    foreach ( devicePort *i , mySockets )
        if ( i != sender && i->isConnect() )
            i->pushToSend( fr->copy() );
    delete fr;
#endif    
}
