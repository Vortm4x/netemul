#include "boxdevice.h"

boxDevice::boxDevice()
{
    myManual = false;
}

boxDevice::~boxDevice()
{
}

bool boxDevice::setSocketCount(int n)
{
    QList<devicePort*> l = sockets();
    foreach( devicePort *i , l )
        if ( i->isConnect() ) return false;
    int i,t = sockets().count();
    if ( t == n ) return false;
    if ( t < n ) {
        for ( i = t+1 ; i <= n; i++)
            addInterface( trUtf8("LAN%1").arg(i),0);
    }
    else {
        foreach( devicePort *i , l)
            removeSocket(i);
        for (i = 1 ; i <=n ; i++)
            addInterface( trUtf8("LAN%1").arg(i),0);
    }
    return true;
}


