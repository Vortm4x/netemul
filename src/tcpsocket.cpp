#include "tcpsocket.h"
#include "ippacket.h"
#include "smartdevice.h"
#include "routemodel.h"

tcpSocket::tcpSocket(smartDevice *d,ipAddress a, quint16 sp, quint16 rp)
{
    sender = sp;
    receiver = rp;
    dev = d;
    dest = a;
    seq = qrand()%Sequence;
    inputTime = 0;
    lastNum = 0;
}

void tcpSocket::setSize(int s)
{
    for ( quint32 i = seq; i < seq + s-1; i++ ) {
        tcpPacket p = createPacket( i, 0, tcpPacket::NO_FLAGS);
        buffer.insert(i,p);
    }
    tcpPacket p = createPacket(seq + s, 0, tcpPacket::FIN);
    buffer.insert(seq + s, p);
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
    routeRecord *r = dev->myRouteTable->recordAt(dest);
    if ( !r ) return;
    if ( r->gateway != r->out ) gw = r->gateway;
    p.setSender(r->out);
    p.setReceiver(dest);
    p.setUpProtocol(smartDevice::TCP);
    dev->ipToAdapter(r->out)->sendPacket(p,gw);
}

void tcpSocket::treatPacket(ipPacket p)
{
    tcpPacket tcp(p.unpack());
    if ( tcp.flag() == tcpPacket::ACK ) {
        if ( seq == tcp.ack() ) timeout = 2*time;
        QMap<quint32,tcpPacket>::iterator i = buffer.begin();
        while( i != buffer.end() ) {
            if ( i.key() < tcp.ack() ) buffer.remove(i.key());
            ++i;
        }
        if ( buffer.isEmpty() ) {
            timeout = 0;
            return;
        }
        for ( int j = 0; j < tcpPacket::Window; j++ ) {
            tcpPacket t = buffer.value( j + tcp.ack() );
            ipPacket a;
            a.pack( t.toData() );
            sendMessage(a);
        }
    }
    else {
        inputTime = time;
        lastNum = tcp.sequence();
    }
}

tcpPacket tcpSocket::createPacket(quint32 sequence, quint32 ack, quint8 flag) const
{
    tcpPacket t;
    t.setSender(sender);
    t.setReceiver(receiver);
    t.setSequence( sequence );
    t.setAck(ack);
    t.setFlag(flag);
    t.setWindow(tcpPacket::Window);
    return t;
}

void tcpSocket::confirmConnection(ipPacket p)
{
    tcpPacket tcp(p.unpack());
    tcpPacket t = createPacket(0, tcp.sequence(), tcpPacket::ACK);
    ipPacket a;
    a.pack(t.toData());
    sendMessage(a);
}

void tcpSocket::secondEvent()
{
    time++;
    if ( lastNum && (time - inputTime) >= 2 ) {
        tcpPacket t = createPacket( 0, lastNum + 1, tcpPacket::ACK);
        ipPacket a;
        a.pack(t.toData());
        sendMessage(a);
        inputTime = 0;
        lastNum = 0;
    }
}
