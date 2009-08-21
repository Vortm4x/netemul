#include "tcpsocket.h"
#include "ippacket.h"
#include "smartdevice.h"
#include "routemodel.h"

tcpSocket::tcpSocket(smartDevice *d,ipAddress a)
{
    dev = d;
    dest = a;
}

void tcpSocket::setConnection() const
{
    ipPacket p;
    tcpPacket t = createPacket(tcpPacket::User, tcpPacket::User, qrand()%Sequence, 0, tcpPacket::NO_FLAGS);
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
        ipPacket a;
        for ( int i = 0; i < size; i++ ) sendMessage(a);
    }
}

tcpPacket tcpSocket::createPacket(quint16 sender, quint16 receiver, quint32 sequence, quint32 ack, quint8 flag) const
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
    tcpPacket t = createPacket(tcpPacket::User, tcpPacket::User, 0, tcp.sequence()+1, tcpPacket::ACK);
    ipPacket a;
    a.pack(t.toData());
    sendMessage(a);
}
