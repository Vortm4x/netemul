#include "switchchip.h"
#include "deviceport.h"
#include "switchmodel.h"
#include "frame.h"

switchChip::switchChip(int c /* = 4 */ ) : boxChip(c)
{
    switchTable = new switchModel;
}

switchChip::~switchChip()
{
    delete switchTable;
}

void switchChip::receiveEvent(frame *fr,devicePort *sender)
{
    switchTable->contains( fr->sender() , sender );

    devicePort *t = switchTable->portWithMac( fr->receiver() );
    if ( t && t->isConnect() ) {
        t->pushToSend(fr);
        return;
    }
    foreach ( devicePort *i , mySockets ) {
        if ( i != sender && i->isConnect() )
            i->pushToSend(fr->copy());
    }
    delete fr;
}

void switchChip::updateMac()
{

}

void switchChip::addToSwitchTable(const macAddress &m , const QString &p, int mode , int time)
{
    QString t = p;
    t.remove(0,3);
    switchTable->addToTable( m , mySockets.at( t.toInt() - 1 ) , mode , time );
}

