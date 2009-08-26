#include "abstractsocket.h"
#include "smartdevice.h"

abstractSocket::abstractSocket(smartDevice *d)
{
    dev = d;
    dev->mySockets << this;
}

abstractSocket::~abstractSocket()
{
    dev->mySockets.removeOne(this);
}


bool abstractSocket::isOurData(ipAddress address, quint16 port)
{
    if ( myBind.isEmpty() || myBind == address )
        if ( port == myBindPort ) return true;
    return false;
}
