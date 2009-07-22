#include "boxchip.h"
#include <QStringList>

#include "deviceport.h"


boxChip::boxChip(int n /* = 4 */)
{
    for ( int i = 0 ; i < n ; i++ ) {
        devicePort *t = new devicePort;
        mySockets.push_back(t);
    }
}

QStringList boxChip::sockets() const
{
    QStringList t;
    for ( int  i = 0 ; i < mySockets.size() ; i++ )
        t << QObject::tr("LAN%1").arg(i+1);
    return t;
}

bool boxChip::setSocketsCount(int n)
{
    int i,t = mySockets.size();
    if ( t <= n ) {
        for ( i = t ; i < n ; i++ ) {
            devicePort *t = new devicePort;
            mySockets.push_back(t);
        }
    }
    else {
        for ( i = n ; i < t ; i++ )
            if ( mySockets.at(i)->isConnect() ) return false;
        for ( i = t-1 ; i >= n ; i-- ) {
            delete mySockets[i];
            mySockets.pop_back();
        }
    }
    return true;
}

QString boxChip::socketName(const cableDev *c) const
{
#ifndef __TESTING__
    for ( int i = 0 ; i < mySockets.size() ; i++ )
        if ( mySockets.at(i)->isCableConnect(c) ) return QObject::tr("LAN%1").arg(i+1);
#endif
    return QString();
}

void boxChip::addConnection(const QString &port , cableDev *c)
{
    QString t = port;
    t.remove(0,3);
#ifndef __TESTING__
    mySockets.at( t.toInt() -1 )->setConnect(true,c);
#endif
}

bool boxChip::isConnectSocket(const QString &str) const
{
    return socket(str)->isConnect();
}

const devicePort* boxChip::socket(const QString &name) const
{
    QString t = name;
    t.remove(0,3);
    return mySockets[ t.toInt()-1 ];
}
