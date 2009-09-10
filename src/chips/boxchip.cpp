/****************************************************************************************
** NetEmul - program for simulating computer networks.
** Copyright © 2009 Semenov Pavel and Omilaeva Anastasia
**
** NetEmul is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** NetEmul is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with the NetEmul; if not, write to the Free
** Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
** 02111-1307 USA.
****************************************************************************************/
#include "boxchip.h"
#include <QStringList>

#include "deviceport.h"


boxChip::boxChip(int n /* = 4 */)
{
    for ( int i = 0 ; i < n ; i++ )
        addSocket(i+1);
}

boxChip::~boxChip()
{
    qDeleteAll(mySockets);
    mySockets.clear();
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
        for ( i = t ; i < n ; i++ )
            addSocket(i+1);
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

void boxChip::deciSecondTimerEvent()
{
#ifndef __TESTING__
    foreach ( devicePort *i , mySockets ) {
        i->queueEvent();
        if ( i->hasReceive() ) {
            frame t = i->popFromReceive();
            receiveEvent(t,i);
        }
    }
#endif
}

void boxChip::addSocket(int n)
{
    devicePort *t = new devicePort(n);
    mySockets.push_back(t);
}

bool boxChip::isBusy() const
{
    foreach ( devicePort *i , mySockets )
        if ( i->isBusy() ) return true;
    return false;
}

void boxChip::setCheckedSocket(const QString &port)
{
    foreach ( devicePort *i, mySockets )
        i->setChecked( tr("LAN%1").arg(i->num()) == port );
}

int boxChip::trafficDigit() const
{
    int sum = 0;
    foreach ( devicePort *i, mySockets )
        sum += i->trafficDigit();
    return sum;
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


