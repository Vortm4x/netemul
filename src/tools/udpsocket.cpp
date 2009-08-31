#include "udpsocket.h"
#include "smartdevice.h"
#include "routemodel.h"
#include "udppacket.h"

udpSocket::udpSocket(smartDevice *d, quint16 port) : abstractSocket(d)
{
    myBindPort = port;
}

void udpSocket::write(ipAddress address, quint16 port, QByteArray data)
{
    ipAddress gw;
    routeRecord *r = dev->myRouteTable->recordAt(address);
    if ( !r ) return;
    if ( r->gateway != r->out ) gw = r->gateway;
    while ( quint32 size = data.size() ) {
        QByteArray tempArray;
        if ( size >= PACKET_SIZE ) tempArray = data.left(PACKET_SIZE);
        else tempArray = data;
        data.remove(0,tempArray.size());
        ipPacket p( r->out, address );
        p.setUpProtocol(ipPacket::udp);
        udpPacket udp;
        udp.setReceiver(port);
        udp.setSender(myBindPort);
        udp.pack(tempArray);
        p.pack(udp.toData());
        dev->ipToAdapter(r->out)->sendPacket(p,gw);
    }
}

void udpSocket::treatPacket(ipPacket p)
{
    udpPacket udp(p.unpack());
    emit readyRead( udp.unpack() );
}

