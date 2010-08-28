#include "spoofingprogramm.h"
#include "spoofingproperty.h"
#include "udpsocket.h"
#include "smartdevice.h"
#include "arpmodel.h"
#include "arppacket.h"

#include <QtDebug>

spoofingProgramm::spoofingProgramm()
{
    myName = tr("Spoofing");
    isReady = false;
    hasAttack = false;
}

void spoofingProgramm::incTime()
{
    static int n = 0;
    if ( !isReady && ++n%10 == 0 ) {
        if ( myServerIp.isEmpty() || myClientIp.isEmpty() ) return;
        udpSocket socket( this->device() , SmartDevice::User );
        QByteArray temp(1,'j');
        socket.write( myClientIp , SmartDevice::User , temp );
        socket.write( myServerIp , SmartDevice::User , temp );
        n = 0;
    }

    if ( !hasAttack && isReady ) {
        sendAnswers();
        hasAttack = true;
    }
}

void spoofingProgramm::setDevice(SmartDevice *s)
{
    programmRep::setDevice(s);
    if ( !s ) return;
    foreach ( Interface *i , myDevice->interfaces() ) {
        connect( i->arpTable() , SIGNAL(tableChanged(arpRecord*)) , this , SLOT(execute(arpRecord*)) );
        arpRecord *t = i->arpTable()->recordAt(myClientIp);
        if ( t ) {
            myClientMac = t->mac;
        }
        t = i->arpTable()->recordAt(myServerIp);
        if ( t ) {
            myServerMac = t->mac;
        }
    }
    if ( !myClientMac.isEmpty() && !myServerMac.isEmpty() ) {        
        isReady = true;
        qDebug() << isReady;
    }
}

void spoofingProgramm::execute(arpRecord *record)
{
    if ( record->ip == myClientIp ) {
        myClientMac = record->mac;
        isReady = true;
    }
    if ( record->ip == myServerIp ) {
        myServerMac = record->mac;
        isReady = true;
    }
}

void spoofingProgramm::sendAnswers()
{
    sendOneAnswer( myServerIp , myClientIp , myClientMac );
    sendOneAnswer( myClientIp , myServerIp , myServerMac );
}

void spoofingProgramm::sendOneAnswer(IpAddress sender, IpAddress receiver, macAddress receiverMac)
{
    arpPacket arp;
    Interface *t = myDevice->ipToAdapter( myDevice->findInterfaceIp( receiver ) );
    Q_ASSERT( t != 0 );
    arp.setReceiverIp( receiver );
    arp.setReceiverMac( receiverMac );
    arp.setSenderIp( sender );
    arp.setSenderMac( t->mac() );
    arp.setType( arpPacket::response );
    frame f;
    f.pack( arp.toData() );
    f.setReceiver( receiverMac );
    f.setSender( t->mac() );
    f.setType( frame::arp );
    f.setDifferent(frame::NORMAL);
    t->pushToSocket(f);
}

void spoofingProgramm::write(QDataStream &stream) const
{
    stream << SPOOFING;
    programmRep::write(stream);
}

void spoofingProgramm::read(QDataStream &stream)
{
    programmRep::read(stream);
}

void spoofingProgramm::showProperty()
{
    spoofingProperty *d = new spoofingProperty;
    d->setProgramm(this);
    d->show();
}
