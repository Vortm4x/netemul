#include "socketfactory.h"
#include "tcpsocket.h"
#include "udpsocket.h"

SocketFactory::SocketFactory()
{
}

AbstractSocket* SocketFactory::getTempSocket(SmartDevice *dev, quint16 port, int type)
{
    if ( type == UDP ) {
        return new UdpSocket(dev,port);
    } else {
        TcpSocket *tcp = new TcpSocket(dev,port);
        QObject::connect(tcp, SIGNAL(writeEnd()), tcp, SLOT(deleteLater()));
        QObject::connect(tcp, SIGNAL(receiveEnd()), tcp, SLOT(deleteLater()));
        return tcp;
    }
    return 0;
}

AbstractSocket* SocketFactory::getTCPSocket(SmartDevice *dev, quint16 port)
{
    TcpSocket *tcp = new TcpSocket(dev,port);
    return tcp;
}
