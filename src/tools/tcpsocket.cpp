#include "tcpsocket.h"
#include "ippacket.h"
#include "smartdevice.h"
#include "routemodel.h"

tcpSocket::tcpSocket(smartDevice *d,quint16 port) : abstractSocket(d)
{
    myBindPort = port;
    isn = qrand()%Sequence;
    inputTime = 0;
    lastNum = 0;
    receiveIsn = 0;
    state = NONE;
    buffer.clear();
}

tcpSocket::~tcpSocket()
{   
    qDebug("I dead!");
}

void tcpSocket::write(ipAddress a, quint16 p, QByteArray data)
{
    myBind = a;
    myReceiverPort = p;    
    buffer = data;
    setConnection();
}

void tcpSocket::setConnection()
{
    time = 0;
    timeout = 0;   
    tcpPacket t = createPacket(isn, 0, tcpPacket::SYN);
    sendMessage(t);
    state = WAIT_RESPONSE;
}

void tcpSocket::sendMessage(tcpPacket t) const
{
    ipPacket p;
    p.pack(t.toData());
    ipAddress gw;
    routeRecord *r = dev->myRouteTable->recordAt(myBind);
    if ( !r ) return;
    if ( r->gateway != r->out ) gw = r->gateway;
    p.setSender(r->out);
    p.setReceiver(myBind);
    p.setUpProtocol(smartDevice::TCP);
    dev->ipToAdapter(r->out)->sendPacket(p,gw);
}

void tcpSocket::treatPacket(ipPacket p)
{
    tcpPacket tcp(p.unpack());
    if ( state == NONE && tcp.flag() != tcpPacket::SYN ) { deleteLater(); return; }

    if ( tcp.flag() == tcpPacket::ACK) { receiveAck(tcp); return; }

    if ( tcp.flag() == tcpPacket::NO_FLAGS ) {
        if ( state == R_WAIT ) {
            tcpPacket a = createPacket(0,0,tcpPacket::RST);
            sendMessage(a);
            return;
        }
        inputTime = time;
        lastNum = tcp.sequence();
        return;
    }
    if ( tcp.flag() == tcpPacket::FIN ) { lastNum = tcp.sequence(); sendAck(); emit receiveEnd(); }
    if ( tcp.flag() == (tcpPacket::SYN | tcpPacket::ACK) ) { receiveSynAck(tcp); return; }
    if ( tcp.flag() == tcpPacket::SYN ) {
        confirmConnection(p);
        return;
    }
    if ( tcp.flag() == tcpPacket::RST ) { deleteLater(); return; }
}

void tcpSocket::receiveSynAck(tcpPacket t)
{
    if ( state != WAIT_RESPONSE ) return;
    timeout = 2*time;
    if ( t.ack() != isn ) return;
    isn = t.sequence();
    tcpPacket a = createPacket(0, t.sequence(), tcpPacket::ACK);
    sendMessage(a);
    sendWindow();
}

void tcpSocket::receiveAck(tcpPacket t)
{
    if ( state == R_WAIT ) { state = RECEIVE; return; }
    if ( state != WAIT_ACK ) return;
    panicTime = 0;
    receiveIsn = t.ack();
    int r = (receiveIsn-sendIsn)*1024;
    buffer.remove(0,r);
    if ( buffer.isEmpty() ) {
        timeout = 0;
        emit writeEnd();
        return;
    }
    sendWindow();
}

tcpPacket tcpSocket::createPacket(quint32 sequence, quint32 ack, quint8 flag) const
{
    tcpPacket t;
    t.setSender(myBindPort);
    t.setReceiver(myReceiverPort);
    t.setSequence( sequence );
    t.setAck(ack);
    t.setFlag(flag);
    t.setWindow(tcpPacket::Window);
    return t;
}

void tcpSocket::confirmConnection(ipPacket p)
{    
    tcpPacket tcp(p.unpack());
    myReceiverPort = tcp.sender();
    myBind = p.sender();
    tcpPacket t = createPacket(isn, tcp.sequence(), tcpPacket::SYN | tcpPacket::ACK);
    sendMessage(t);
    state = R_WAIT;
}

void tcpSocket::sendWindow()
{
    panicTime = 0;
    sendIsn = isn;
    QByteArray data = buffer.left(tcpPacket::Window);
    int count = 0;
    int size = data.size();
    while ( size >= PACKET_SIZE ) {
        count++;        
        size -= PACKET_SIZE;
    }
    for ( int j = 0; j < count; j++ ) {
        tcpPacket t = createPacket(isn,0,tcpPacket::NO_FLAGS);
        if ( buffer.size()<=tcpPacket::Window && j==count-1) t.setFlag(tcpPacket::FIN);
        t.pack(data.left(PACKET_SIZE));
        data.remove(0,PACKET_SIZE);
        sendMessage(t);
        isn++;
    }
    state = WAIT_ACK;
}

void tcpSocket::sendAck()
{
     tcpPacket t = createPacket( 0, lastNum + 1, tcpPacket::ACK);
     sendMessage(t);
     inputTime = 0;
     lastNum = 0;
}

void tcpSocket::secondEvent()
{
    time++;
    if ( time > 45 && state == WAIT_RESPONSE ) {
        deleteLater();
        return;
    }
    if ( ++panicTime == timeout ) {
        sendWindow();
        return;
    }
    if ( lastNum && (time - inputTime) >= 2 ) sendAck();
}
