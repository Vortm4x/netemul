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


BoxChip::BoxChip(QObject *parent) : AbstractChip(parent)
{
}

BoxChip::~BoxChip()
{
    qDeleteAll(mySockets);
    mySockets.clear();
}

QStringList BoxChip::sockets() const
{
    QStringList t;
    for ( int  i = 0 ; i < mySockets.size() ; i++ )
        t << QObject::tr("LAN%1").arg(i+1);
    return t;
}

bool BoxChip::setSocketsCount(int n)
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
    emit socketsCountChanged();
    return true;
}

QString BoxChip::socketName(const Cable *c) const
{
    for ( int i = 0 ; i < mySockets.size() ; i++ )
        if ( mySockets.at(i)->isCableConnect(c) ) return QObject::tr("LAN%1").arg(i+1);
    return QString();
}

void BoxChip::onCableConnected(Cable *cable)
{
}

DevicePort* BoxChip::findPortByName(const QString &name) const
{
    QString t = name;
    t.remove(0,3);
    int n = t.toInt() - 1;
    if ( n >= 0 && n < mySockets.size()  ) {
        return mySockets.at(n);
    } else {
        qDebug("WARNING! %s - %d",qPrintable(name),n);
    }
    return 0;
}

bool BoxChip::isConnectSocket(const QString &str) const
{
    return mySockets[ str.mid(3).toInt() - 1 ]->isConnect();
}

DevicePort* BoxChip::socket(const QString &name)
{
    return mySockets[ name.mid(3).toInt()-1 ];
}

void BoxChip::deciSecondTimerEvent()
{
#ifndef __TESTING__
    foreach ( DevicePort *i , mySockets ) {
        i->queueEvent();
        if ( i->hasReceive() ) {
            frame t = i->popFromReceive();
            receiveEvent(t,i);
        }
    }
#endif
}

void BoxChip::addSocket(int n)
{
    DevicePort *t = new DevicePort(n,this);
    connect( t ,SIGNAL(cableConnected(Cable*)) , SLOT(onCableConnected(Cable*)) );
    mySockets.push_back(t);
}



bool BoxChip::isBusy() const
{
    foreach ( DevicePort *i , mySockets )
        if ( i->isBusy() ) return true;
    return false;
}

bool BoxChip::isConnect() const
{
    foreach ( DevicePort *i , mySockets ) {
        if ( i->isConnect() ) {
            return true;
        }
    }
    return false;
}

QList<Cable*> BoxChip::cableList() const
{
    QList<Cable*> list;
    foreach ( DevicePort *i , mySockets ) {
        if ( i->isConnect() ) {
            list << i->cable();
        }
    }
    return list;
}

QString BoxChip::portToString(DevicePort *port) const
{
    return QObject::tr("LAN%1").arg(port->num());
}

int BoxChip::trafficDigit() const
{
    int sum = 0;
    foreach ( DevicePort *i, mySockets )
        sum += i->trafficDigit();
    return sum;
}

#ifndef __TESTING__
void BoxChip::setCheckedSocket(const QString &port)
{
    foreach ( DevicePort *i, mySockets )
        i->setChecked( tr("LAN%1").arg(i->num()) == port );
}

void BoxChip::write(QDataStream &stream) const
{
    AbstractChip::write(stream);
    stream << mySockets.size();
}

void BoxChip::read(QDataStream &stream)
{
    AbstractChip::read(stream);
    int n;
    stream >> n;
    setSocketsCount(n);
}

#endif


