#include <QtCore/QList>
#include <QtGui/QMessageBox>
#include "interface.h"
#include "deviceport.h"
#include "appsetting.h"
#include "arpmodel.h"
//#include <QtDebug>

interface::interface(const QString &name) : myName(name)
{
    mySocket = new devicePort;
    myArpTable = new arpModel;
}

interface::~interface()
{
    delete mySocket;
    delete myArpTable;
}

void interface::pushToSocket(frame &f)
{
    checkSend(f);
    emit sendData(f,myName);
    mySocket->pushToSend(f);
}

void interface::receiveEvent(frame &fr,devicePort*)
{
    checkReceive(fr);
    emit receiveData(fr,myName);
    if ( fr.type() == frame::ip ) {
        ipPacket p(fr.unpack());
        receiveIp(p);
    }
    if ( fr.type() == frame::arp ) {
        arpPacket p(fr.unpack());
        receiveArp(p);
    }
}

void interface::sendBroadcast(ipPacket &p)
{
    frame f = createFrame( macAddress("FF:FF:FF:FF:FF:FF") , frame::ip );
    f.setDifferent( frame::broadcast );
    f.pack( p.toData() );
    pushToSocket(f);
}

void interface::sendPacket(ipPacket &p,ipAddress gw /* = ipAddress("0.0.0.0") */ )
{
    if ( p.isBroadcast( myMask ) ) {
        sendBroadcast(p);
        return;
    }
    ipAddress t;    
    if ( gw.isEmpty() ) t = p.receiver();
    else t = gw;
    arpRecord *a = myArpTable->recordAt(t);
    if ( a ) {
        frame f = createFrame( a->mac , frame::ip );
        a->time = 0; // Стартуем заново время жизни arp записи
        f.pack( p.toData() );
        pushToSocket(f);
        return;
    }
    if ( myWaits.contains( t ) ) {
        myWaits.insert(t,p);
        return;
    }
    sendArpRequest(t);
    myWaits.insert(t,p);
}

frame interface::createFrame( macAddress receiverMac, int t)
{
    frame f;
    f.setSender(myMac);
    f.setReceiver(receiverMac);
    f.setType(t);
    return f;
}

void interface::receiveIp(ipPacket &ip)
{
    buffer.enqueue(ip);
}

void interface::receiveArp(arpPacket &arp)
{
    if ( arp.type() == arpPacket::response ) {
        if ( arp.receiverIp() == arp.senderIp() ) {
            QMessageBox::warning(0, QObject::tr("The network is not working correctly"),
                                 QObject::tr("The network found a match ip-address! "),
                                 QMessageBox::Ok, QMessageBox::Ok);
        }
        myArpTable->addToTable(  arp.senderIp() , arp.senderMac() , arpModel::dinamicMode );
        QList<ipPacket> packets = myWaits.values( arp.senderIp() );
        for ( int i = 0; i < packets.size() ; i++ ) sendPacket( packets[i] , arp.senderIp() );
        Q_ASSERT( packets.size() == myWaits.remove( arp.senderIp() ) );
    }
    else {
        arpRecord *t = myArpTable->addToTable(arp.senderIp() , arp.senderMac() , arpModel::dinamicMode );
        if ( arp.receiverIp() == myIp ) sendArpResponse(t->mac, t->ip);
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

void interface::setChecked(bool b)
{
    mySocket->setChecked(b);
}

void interface::deciSecondEvent()
{
    mySocket->queueEvent();
    if ( mySocket->hasReceive() ) {
        frame f = mySocket->popFromReceive();
        receiveEvent( f, mySocket );
    }
}

void interface::secondEvent()
{
    myArpTable->update();
}

void interface::sendArpRequest(ipAddress a)
{
    if ( a.isEmpty() ) return;
    arpPacket p(  macAddress() , myMac , a , myIp , arpPacket::request );
    macAddress m;
    m.setBroadcast();
    frame f = createFrame(m, frame::arp);
    f.setDifferent(frame::broadcast);
    f.pack(p.toData());
    pushToSocket(f);
}

void interface::sendArpResponse(macAddress m, ipAddress a)
{
    arpPacket p(m, myMac, a, myIp, arpPacket::response);
    frame f = createFrame(m, frame::arp);
    f.pack(p.toData());
    f.setDifferent(frame::normal);
    pushToSocket(f);
}

void interface::write(QDataStream &stream) const
{
    abstractChip::write(stream);
    stream << myName;
}

void interface::read(QDataStream &stream)
{
    abstractChip::read(stream);
    stream >> myName;
}

bool interface::isBusy() const
{
    return mySocket->isBusy() || !buffer.isEmpty();
}


