#include "interface.h"
#include "frame.h"
#include "arppacket.h"
#include "deviceport.h"
#include <QList>
#include <QMessageBox>

interface::interface(const QString &name) : myName(name)
{
    mySocket = new devicePort;
}

interface::~interface()
{
    delete mySocket;
    clearArp();
}

void interface::receiveEvent(frame *fr)
{
    if ( fr->type() == frame::ip ) {
        ipPacket *p = new ipPacket(fr->unpack());
        receiveIp(p);
    }
    if ( fr->type() == frame::arp ) {
        arpPacket *p = new arpPacket(fr->unpack());
        receiveArp(p);
    }
}

void interface::sendBroadcast(ipPacket *p)
{
    frame *f = createFrame( macAddress("FF:FF:FF:FF:FF:FF") , frame::ip );
    f->setDifferent( frame::broadcast );
    f->pack( p->toData() );
    mySocket->pushToSend(f);
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
            frame *f = createFrame( i->mac , frame::ip );
            i->time = 0; // Стартуем заново время жизни arp записи
            f->pack( p->toData() );
            mySocket->pushToSend(f);
            delete p;
            return;
        }
    if ( myWaits.contains( t ) ) {
        myWaits.insert(t,p);
        return;
    }
    sendArpRequest(t);
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

frame* interface::createFrame( macAddress receiverMac, int t)
{
    frame *f = new frame;
    f->setSender(myMac);
    f->setReceiver(receiverMac);
    f->setType(t);
    return f;
}

void interface::receiveIp(ipPacket *ip)
{
    buffer.enqueue(ip);
}

void interface::updateArp()
{
    foreach ( arpRecord *i, myArpTable ) {
        if ( i->mode == staticMode ) continue;
        if ( ++i->time == 1200 ) {
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

void interface::receiveArp(arpPacket *arp)
{
    if ( arp->type() == arpPacket::response ) {
        if ( arp->receiverIp() == arp->senderIp() ) {
            QMessageBox::warning(0, QObject::trUtf8("Некорректная работа сети"),
                                 QObject::trUtf8("В сети обнаружено совпадение ip-адресов!"),QMessageBox::Ok, QMessageBox::Ok);
        }
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
        if ( arp->receiverIp() == myIp ) sendArpResponse(t->mac, t->ip);
    }
}

bool interface::isConnect() const
{
    return mySocket->isConnect();
}

void interface::setConnect(bool b,cableDev *c)
{
    mySocket->setConnect(b,c);
}

bool interface::isCableConnect(const cableDev *c) const
{
    return mySocket->isCableConnect(c);
}

void interface::deciSecondEvent()
{
    mySocket->queueEvent();
    frame *f = mySocket->popFromReceive();
    if ( f ) receiveEvent(f);
}

void interface::sendArpRequest(ipAddress a)
{
    if ( a.isEmpty() ) return;
    arpPacket p(  macAddress() , myMac , a , myIp , arpPacket::request );
    macAddress m;
    m.setBroadcast();
    frame *f = createFrame(m, frame::arp);
    f->setDifferent(frame::broadcast);
    f->pack(p.toData());
    mySocket->pushToSend(f);
}

void interface::sendArpResponse(macAddress m, ipAddress a)
{
    arpPacket p(m, myMac, a, myIp, arpPacket::response);
    frame *f = createFrame(m, frame::arp);
    f->pack(p.toData());
    f->setDifferent(frame::normal);
    mySocket->pushToSend(f);
}


