#include "switchchip.h"
#include "deviceport.h"

switchChip::switchChip(int c /* = 4 */ )
{
    for ( int i = 0; i < c; i++ ) {
        devicePort *p = new devicePort;
        mySockets << p;
    }
}

void switchChip::receiveEvent(frame *fr,devicePort *sender)
{
    bool contains = false;
    foreach ( macRecord *i , mySwitchTable )
        if ( i->mac == fr->sender() ) {
            if ( i->port != sender && i->mode != staticMode ) i->port = sender;
            contains = true;
            break;
        }
    if ( !contains ) addToTable(fr->sender() , sender  , dinamicMode , 0);

    foreach ( macRecord *i , mySwitchTable )
        if ( i->mac == fr->receiver() ) {
            if ( i->port->isConnect() && i->port != sender ) {
                    i->time = 0;
                    i->port->pushToSend(fr);
            }
            return;
        }
    foreach ( devicePort *item , mySockets ) {
        if ( item != sender && item->isConnect() )
            item->pushToSend(fr->copy());
    }
    delete fr;
}

macRecord* switchChip::addToTable(macAddress mac ,devicePort *p , int mode , int time)
{
    foreach ( macRecord *i , mySwitchTable )
        if ( i->mac == mac ) return NULL;
    macRecord *t = new macRecord;
    t->time = time;
    t->mode = mode;
    t->port = p;
    t->mac = mac;
    mySwitchTable << t;
    return t;
}

void switchChip::deleteFromTable(macAddress mac)
{
    foreach ( macRecord *i , mySwitchTable )
        if ( i->mac == mac ) {
            mySwitchTable.removeOne(i);
            delete i;
            return;
        }
}

void switchChip::updateMac()
{
    foreach( macRecord *i, mySwitchTable)
        if ( ++i->time == 30 ) deleteFromTable(i->mac);
}

