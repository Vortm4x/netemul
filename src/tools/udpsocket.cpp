#include "udpsocket.h"
#include "smartdevice.h"
#include "routemodel.h"
#include "udppacket.h"

udpSocket::udpSocket(smartDevice *d ,ipAddress address, quint16 port) : abstractSocket(d)
{
    dest = address;
    myPort = port;
}

void udpSocket::write(QByteArray data)
{
    ipAddress gw;
    routeRecord *r = dev->myRouteTable->recordAt(dest);
    if ( !r ) return;
    if ( r->gateway != r->out ) gw = r->gateway;
    while ( quint32 size = data.size() ) {
        QByteArray tempArray;
        if ( size >= PACKET_SIZE ) tempArray = data.left(PACKET_SIZE);
        else tempArray = data;
        data.remove(0,tempArray.size());
        ipPacket p( r->out, dest );
        p.setUpProtocol(ipPacket::udp);
        udpPacket udp;
        udp.setReceiver(myPort);
        udp.setSender(myPort);
        udp.pack(tempArray);
        p.pack(udp.toData());
        dev->ipToAdapter(r->out)->sendPacket(p,gw);
    }
}

