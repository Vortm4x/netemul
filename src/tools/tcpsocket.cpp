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
    buffer.clear();
}

tcpSocket::~tcpSocket()
{   
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
    if ( tcp.flag() == (tcpPacket::SYN | tcpPacket::ACK) ) {
        timeout = 2*time;        
        if ( tcp.ack() != isn ) return;
        isn = tcp.sequence();
        tcpPacket a = createPacket(0, tcp.sequence(), tcpPacket::ACK);
        sendMessage(a);
        sendWindow();
        return;
    }
    if ( tcp.flag() == tcpPacket::SYN ) {
        confirmConnection(p);
        return;
    }
    if ( tcp.flag() == tcpPacket::ACK) {
        panicTime = 0;
        receiveIsn = tcp.ack();
        int r = (receiveIsn-sendIsn)*1024;
        buffer.remove(0,r);
        if ( buffer.isEmpty() && !timeout) return; // Если мы принимающая сторона !!!ПЕРЕПИСАТЬ!!!
        else if ( buffer.isEmpty() ) {
            timeout = 0;
            emit writeEnd();
            return;
        }
        sendWindow();
    }
    else {
        inputTime = time;
        lastNum = tcp.sequence();
        if ( tcp.flag() == tcpPacket::FIN ) { sendAck(); emit receiveEnd(); }
    }
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
        //if ( buffer.size()<=tcpPacket::Window && j==count-1) t.setFlag(tcpPacket::FIN);
        t.pack(data.left(PACKET_SIZE));
        data.remove(0,PACKET_SIZE);
        sendMessage(t);
        isn++;
    }
}

void tcpSocket::sendAck()
{
    if ( lastNum && (time - inputTime) >= 2 ) {
        tcpPacket t = createPacket( 0, lastNum + 1, tcpPacket::ACK);
        sendMessage(t);
        inputTime = 0;
        lastNum = 0;
    }
}

void tcpSocket::secondEvent()
{
    time++;
//    if ( time > 45 && !isConnected ) {
//        deleteLater();
//        return;
//    }
    if ( ++panicTime == timeout ) {
        sendWindow();
        return;
    }
    sendAck();
}
