#include "boxchip.h"

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

devicePort* boxChip::socket(const QString &name)
{
    QString t = name;
    t.remove(0,3);
    return mySockets.at( t.toInt() );
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

QString boxChip::socketName(devicePort *p) const
{
    for ( int i = 0 ; i < mySockets.size() ; i++ )
        if ( mySockets.at(i) == p ) return QObject::tr("LAN%1").arg(i+1);
    return QString();
}
