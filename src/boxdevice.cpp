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
    foreach( devicePort *i , mySockets )
        if ( i->isConnect() ) return false;
    int i,t = sockets().count();
    if ( t == n ) return false;
    if ( t < n ) {
        for ( i = t+1 ; i <= n; i++)
            addSocket( trUtf8("LAN%1").arg(i));
    }
    else {
        qDeleteAll(mySockets);
        mySockets.clear();
        for (i = 1 ; i <=n ; i++)
            addSocket( trUtf8("LAN%1").arg(i));
    }
    return true;
}


