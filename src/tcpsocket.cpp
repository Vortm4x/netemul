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
    if ( tcp.flag() == tcpPacket::NO_FLAGS ) {
    }
}

tcpPacket tcpSocket::createPacket(quint16 s, quint16 r, quint32 seq, quint32 a, quint8 f) const
{
    tcpPacket t;
    t.setSender(s);
    t.setReceiver(r);
    t.setSequence( seq );
    t.setAck(a);
    t.setFlag(f);
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
