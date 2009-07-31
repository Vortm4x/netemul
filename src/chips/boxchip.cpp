#include "boxchip.h"
#include <QStringList>

#include "deviceport.h"


boxChip::boxChip(int n /* = 4 */)
{
    for ( int i = 0 ; i < n ; i++ ) {
        devicePort *t = new devicePort;
        t->setNum(i+1);
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
            t->setNum(i+1);
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
    for ( int i = 0 ; i < mySockets.size() ; i++ )
        if ( mySockets.at(i)->isCableConnect(c) ) return QObject::tr("LAN%1").arg(i+1);
    return QString();
}

void boxChip::addConnection(const QString &port , cableDev *c)
{
    QString t = port;
    t.remove(0,3);
    mySockets.at( t.toInt() - 1 )->setConnect(true,c);
}

void boxChip::deleteConnection(cableDev *c)
{
    foreach ( devicePort *i , mySockets )
        if ( i->isCableConnect(c) ) {
            i->setConnect(false,0);
            return;
        }
}


bool boxChip::isConnectSocket(const QString &str) const
{
    return mySockets[ str.mid(3).toInt() - 1 ]->isConnect();
}

devicePort* boxChip::socket(const QString &name)
{
    return mySockets[ name.mid(3).toInt()-1 ];
}

#ifndef __TESTING__
void boxChip::deciSecondTimerEvent()
{
    foreach ( devicePort *i , mySockets ) {
        i->queueEvent();
        if ( i->hasReceive() ) {
            frame t = i->popFromReceive();
            receiveEvent(t,i);
        }
    }
}
#endif

bool boxChip::isBusy() const
{
    foreach ( devicePort *i , mySockets )
        if ( i->isBusy() ) return true;
    return false;
}

#ifndef __TESTING__
void boxChip::write(QDataStream &stream) const
{
    abstractChip::write(stream);
    stream << mySockets.size();
}

void boxChip::read(QDataStream &stream)
{
    abstractChip::read(stream);
    int n;
    stream >> n;
    setSocketsCount(n);
}
#endif
