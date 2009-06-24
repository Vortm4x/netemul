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
    if ( !fr->arp() && !fr->packet() ) { delete fr ; return; }
    if ( fr->type() == frame::tIp ) receiveIp( fr->packet() , sender );

    if ( fr->type() == frame::tArp ) receiveArp(fr->arp(), sender);
    delete fr;
}

void interface::sendBroadcast(ipPacket *p)
{
    frame *f = createFrame( myMac , macAddress("FF:FF:FF:FF:FF:FF") , frame::tIp );
    f->setPacket(p);
    addSend(0,1);
    mySocket->addToQueue(f);
}

void interface::sendPacket(ipPacket *p,ipAddress gw)
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
            frame *f = createFrame( myMac , i->mac , frame::tIp );
            i->time = 0; // Стартуем заново время жизни arp записи
            f->setPacket(p);
            addSend(0,1);
            mySocket->addToQueue(f);
            return;
        }
    if ( myWaits.contains( t ) ) {
        myWaits.insert(t,p);
        return;
    }
    macAddress m;
    m.setBroadcast();
    arpPacket *a = createArp( myIp , myMac , t , macAddress() , arpPacket::request );
    frame *f = createFrame( myMac , m ,frame::tArp);
    f->setArp(a);
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

arpPacket* interface::createArp(ipAddress senderIp , macAddress senderMac, ipAddress receiverIp,macAddress receiverMac,int t)
{
    arpPacket *a = new arpPacket;
    a->setReceiverMac( receiverMac );
    a->setSenderIp( senderIp );
    a->setReceiverIp( receiverIp );
    a->setSenderMac( senderMac );
    a->setType(t);
    if ( t == arpPacket::answer ) qDebug() << "send arp: " << senderIp << " answered "  << receiverIp ;
    else qDebug() << "send arp: " << senderIp << " search "  << receiverIp ;
    return a;
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
    qDeleteAll(myArpTable);
    myArpTable.clear();
}

void interface::receiveArp(arpPacket *arp, devicePort *sender)
{
    if ( arp->type() == arpPacket::answer ) {
        addToTable(  arp->senderIp() , arp->senderMac() , dinamicMode );
        QMultiMap<ipAddress,ipPacket*>::iterator i;
        for ( i = myWaits.begin() ; i != myWaits.end() ; ++i ) {
            if ( i.key() == arp->senderIp() ) {
                sendPacket(i.value(),i.key());
                myWaits.remove(i.key() , i.value() );
            }
        }
    }
    else {
        arpRecord *t = addToTable(arp->senderIp() , arp->senderMac() , dinamicMode );
        if ( arp->receiverIp() == myIp ) {
            arp = createArp( myIp , myMac , t->ip , t->mac , arpPacket::answer );
            frame *fr = createFrame( myMac, t->mac, frame::tArp);
            fr->setArp(arp);
            sender->addToQueue(fr);
        }
    }
}


