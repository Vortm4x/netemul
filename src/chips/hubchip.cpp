#include "hubchip.h"
#include "deviceimpl.h"

hubChip::hubChip()
{

}

void hubChip::receiveEvent(frame *fr,devicePort *sender)
{
    foreach ( devicePort *i , myOwner->sockets() ) {
        if ( i != sender && i->isConnect() ) {
            frame *t = new frame;
            *t = *fr;
            i->addToQueue(t);
        }
    }
    delete fr;
}
