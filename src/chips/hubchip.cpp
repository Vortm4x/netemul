#include "hubchip.h"
#include "deviceport.h"

hubChip::hubChip(int n /* = 4 */) : boxChip(n)
{
    foreach ( devicePort *i , mySockets )
        i->setShared(true);
}

void hubChip::receiveEvent(frame &fr,devicePort *sender)
{
    checkReceive(fr);
    emit receiveData(fr,tr("LAN%1").arg(sender->num()));
    foreach ( devicePort *i , mySockets )
        if ( i != sender && i->isConnect() ) {
            checkSend(fr);
            emit sendData(fr, tr("LAN%1").arg(i->num()) );
            i->pushToSend( fr );
        }   
}

void hubChip::detectCollision()
{
    foreach ( devicePort *i , mySockets ) i->startCollision();
}

void hubChip::addSocket(int n)
{
    boxChip::addSocket(n);
    mySockets[n-1]->setShared(true);
}


