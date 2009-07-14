#include "interface.h"
#include "frame.h"
#include "arppacket.h"
#include "smartdevice.h"
#include <QtDebug>
#include <QList>

interface::interface(devicePort *parent, int t )
{
    myType = t;
    setParent(parent);
    mySocket = parent;
    myReceiveFrame  = 0;
}

interface::~interface()
{
    clearArp();
}

void interface::receiveEvent(frame *fr,devicePort *sender)
{
    if ( fr->type() == frame::ip ) {
        ipPacket *p = new ipPacket;
        *fr >> *p;
        receiveIp( p , sender );
    }
    if ( fr->type() == frame::arp ) {
        arpPacket p;
        *fr >> p;
        receiveArp( p , sender);
    }
    delete fr;
}

void interface::sendBroadcast(ipPacket *p)
{
    frame *f = createFrame( myMac , macAddress("FF:FF:FF:FF:FF:FF") , frame::ip );
    f->setDifferent( frame::broadcast );
    *f << *p;
    addSend(0,1);
    mySocket->addToQueue(f);
}

void interface::sendPacket(ipPacket *p,ipAddress gw /* = ipAddress("0.0.0.0") */ )
{
    if ( p->isBroadcast( myMask ) ) {
        sendBroadcast(p);
        return;
    }
    ipAddress t;    
    if ( gw.isEmpty() ) t = p->receiver();
    else t = gw;
    foreach ( arpRecord *i , myArpTable )
        if ( i->ip == t ) {
            frame *f = createFrame( myMac , i->mac , frame::ip );
            i->time = 0; // Стартуем заново время жизни arp записи
            *f << *p;
            addSend(0,1);
            mySocket->addToQueue(f);
            delete p;
            return;
        }
    if ( myWaits.contains( t ) ) {
        myWaits.insert(t,p);
        return;
    }
    macAddress m;
    m.setBroadcast();
    arpPacket a(  macAddress() , myMac , t , myIp , arpPacket::request );
    frame *f = createFrame( myMac , m ,frame::arp);
    f->setDifferent(frame::broadcast);
    *f << a;
    mySocket->addToQueue(f);
    myWaits.insert(t,p);
    return;
}

void interface::removeFromTable(QString ip)
{
    ipAddress a(ip);
    foreach ( arpRecord *i, myArpTable )
        if ( i->ip == a ) {
            myArpTable.removeOne(i);
            delete i;
        }
}

arpRecord* interface::addToTable(ipAddress ip , macAddress mac , int mode )
{
    foreach ( arpRecord *i , myArpTable ) {
        if ( i->ip == ip && i->mac == mac ) return i;
        if ( i->mode != staticMode && (i->ip == ip || i->mac == mac ) ) {
            i->ip = ip;
            i->mac = mac;
            i->mode = mode;
            return i;
        }
    }
    arpRecord *t = new arpRecord;
    t->ip = ip;
    t->mac = mac;
    t->mode = mode;
    t->time = 0;
    myArpTable << t;
    return t;
}

frame* interface::createFrame( macAddress senderMac, macAddress receiverMac, int t)
{
    frame *f = new frame;
    f->setSender(senderMac);
    f->setReceiver(receiverMac);
    f->setType(t);
    return f;
}

void interface::receiveIp(ipPacket *ip , devicePort *sender)
{
    Q_UNUSED(sender);
    addRec(0,1);
    mySmart->receivePacket(ip,this);
}

void interface::updateArp(int u)
{
    foreach ( arpRecord *i, myArpTable ) {
        if ( i->mode == staticMode ) continue;
        if ( ++i->time == u ) {
            myArpTable.removeOne(i);
            delete i;
        }
    }
}

void interface::clearArp()
{
    myIp = ipAddress("0.0.0.0");
    myMask = ipAddress("0.0.0.0");
    qDeleteAll(myArpTable);
    myArpTable.clear();
}

void interface::receiveArp(arpPacket arp, devicePort *sender)
{
    if ( arp.type() == arpPacket::answer ) {
        addToTable(  arp.senderIp() , arp.senderMac() , dinamicMode );
        QMultiMap<ipAddress,ipPacket*>::iterator i;
        for ( i = myWaits.begin() ; i != myWaits.end() ; ++i ) {
            if ( i.key() == arp.senderIp() ) {
                sendPacket(i.value(),i.key());
                myWaits.remove(i.key() , i.value() );
            }
        }
    }
    else {
        arpRecord *t = addToTable(arp.senderIp() , arp.senderMac() , dinamicMode );
        if ( arp.receiverIp() == myIp ) {
            arpPacket a( t->mac , myMac , t->ip ,myIp, arpPacket::answer );
            frame *fr = createFrame( myMac, t->mac, frame::arp);
            *fr << a;
            sender->addToQueue(fr);
        }
    }
}


