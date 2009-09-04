#include "tcpsocket.h"
#include "ippacket.h"
#include "smartdevice.h"
#include "routemodel.h"

tcpSocket::tcpSocket(smartDevice *d,quint16 port) : abstractSocket(d)
{
    myBindPort = port;
    seq = qrand()%Sequence;
    inputTime = 0;
    lastNum = 0;
    isEnd = false;
    isConnected = false;
    buffer.clear();
}

tcpSocket::~tcpSocket()
{   
}

void tcpSocket::write(ipAddress a, quint16 p, QByteArray data)
{
    myBind = a;
    myReceiverPort = p;
    quint32 count = 0;
    while ( quint32 size = data.size() )
        if ( size >= PACKET_SIZE ) {
            count++;
            data.remove(0,PACKET_SIZE);            
        }
    for ( quint32 i = seq; i < seq + count-1; i++ ) {
        tcpPacket t = createPacket( i, 0, tcpPacket::NO_FLAGS);
        buffer << t;
    }
    tcpPacket t = createPacket(seq + count-1, 0, tcpPacket::FIN);
    buffer << t;
    if ( !isConnected ) setConnection();
}

void tcpSocket::setConnection()
{
    time = 0;
    timeout = 0;
    ipPacket p;
    tcpPacket t = createPacket(seq, 0, tcpPacket::NO_FLAGS);
    p.pack(t.toData());
    sendMessage(p);
}

void tcpSocket::sendMessage(ipPacket p) const
{
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
    if ( tcp.flag() == tcpPacket::ACK) {
        panicTime = 0;
        if ( seq == tcp.ack() ) {
            timeout = 2*time;
            isConnected = true;
        }
        QList<tcpPacket>::iterator i = buffer.begin();
        while( i != buffer.end() ) {
            if ( i->sequence() < tcp.ack() ) buffer.erase(i);
            ++i;
        }
        if ( buffer.isEmpty() ) {
            timeout = 0;
            emit writeEnd();
            return;
        }
        sendWindow();
    }
    else {
        if ( !isConnected ) {
            confirmConnection(p);
            return;
        }
        inputTime = time;
        lastNum = tcp.sequence();
        if ( tcp.flag() == tcpPacket::FIN ) isEnd = true;
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
    isConnected = true;
    tcpPacket tcp(p.unpack());
    myReceiverPort = tcp.sender();
    myBind = p.sender();
    tcpPacket t = createPacket(0, tcp.sequence(), tcpPacket::ACK);
    ipPacket a;
    a.pack(t.toData());
    sendMessage(a);
}

void tcpSocket::sendWindow()
{
    panicTime = 0;
    for ( int j = 0; j < tcpPacket::Window; j++ ) {
        if ( j == buffer.size() ) return;
        tcpPacket t = buffer.at(j);
        ipPacket a;
        a.pack( t.toData() );
        sendMessage(a);
    }
}

void tcpSocket::secondEvent()
{
    time++;
    if ( time > 45 && !isConnected ) {
        deleteLater();
        return;
    }
    if ( ++panicTime == timeout ) {
        sendWindow();
        return;
    }
    if ( lastNum && (time - inputTime) >= 2 ) {
        tcpPacket t = createPacket( 0, lastNum + 1, tcpPacket::ACK);
        ipPacket a;
        a.pack(t.toData());
        sendMessage(a);
        inputTime = 0;
        lastNum = 0;
        if (isEnd) emit receiveEnd();
    }
}
