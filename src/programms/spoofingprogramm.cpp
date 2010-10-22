#include "spoofingprogramm.h"
#include "spoofingproperty.h"
#include "smartdevice.h"
#include "arpmodel.h"
#include "arppacket.h"
#include "abstractsocket.h"

#include <QtDebug>

SpoofingProgram::SpoofingProgram(QObject *parent) : Program(parent) ,
                                                    isReady(false) , hasAttack(false)
{    
    myName = tr("Spoofing");
}

void SpoofingProgram::incTime()
{
    static int n = 0;
    if ( !isReady && ++n%10 == 0 ) {
        if ( myServerIp.isEmpty() || myClientIp.isEmpty() ) return;
        AbstractSocket *socket = myDevice->openSocket( SmartDevice::USER_PORT , SmartDevice::UDP );
        QByteArray temp(1,'j');
        socket->write( myClientIp , SmartDevice::USER_PORT , temp );
        socket->write( myServerIp , SmartDevice::USER_PORT , temp );
        n = 0;
        myDevice->disposeSocket(socket);
    }

    if ( !hasAttack && isReady ) {
        sendAnswers();
        hasAttack = true;
    }
}

void SpoofingProgram::setDevice(SmartDevice *s)
{
    Program::setDevice(s);
    if ( !s ) return;
    foreach ( Interface *i , myDevice->interfaces() ) {
        connect( i->arpTable() , SIGNAL(tableChanged(ArpRecord*)) , this , SLOT(execute(ArpRecord*)) );
        ArpRecord *t = i->arpTable()->recordAt(myClientIp);
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

void SpoofingProgram::execute(ArpRecord *record)
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

void SpoofingProgram::sendAnswers()
{
    sendOneAnswer( myServerIp , myClientIp , myClientMac );
    sendOneAnswer( myClientIp , myServerIp , myServerMac );
}

void SpoofingProgram::sendOneAnswer(IpAddress sender, IpAddress receiver, macAddress receiverMac)
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

void SpoofingProgram::write(QDataStream &stream) const
{
    stream << SPOOFING;
    Program::write(stream);
}

void SpoofingProgram::read(QDataStream &stream)
{
    Program::read(stream);
}

void SpoofingProgram::showProperty()
{
    spoofingProperty *d = new spoofingProperty;
    d->setProgramm(this);
    d->show();
}
