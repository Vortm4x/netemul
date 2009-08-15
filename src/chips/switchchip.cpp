#include "switchchip.h"
#include "deviceport.h"
#include "switchmodel.h"

switchChip::switchChip(int c /* = 4 */ ) : boxChip(c)
{
    switchTable = new switchModel;
}

switchChip::~switchChip()
{
    delete switchTable;
}

void switchChip::receiveEvent(frame &fr,devicePort *sender)
{
    checkReceive(fr);
    emit receiveData(fr,tr("LAN%1").arg(sender->num()));

    switchTable->contains( fr.sender() , sender );

    devicePort *t = switchTable->portWithMac( fr.receiver() );
    if ( t && t->isConnect() ) {
        checkSend(fr);
        emit sendData(fr, tr("LAN%1").arg(t->num()));
        t->pushToSend(fr);
        return;
    }
    foreach ( devicePort *i , mySockets ) {
        if ( i != sender && i->isConnect() ) {
            checkSend(fr);
            emit sendData(fr, tr("LAN%1").arg(i->num()));
            i->pushToSend(fr);
        }
    }
}

void switchChip::secondTimerEvent()
{
    switchTable->updateMac();
}


