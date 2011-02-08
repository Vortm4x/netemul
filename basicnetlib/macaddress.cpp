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
#include "macaddress.h"
#include <QStringList>

macAddress::macAddress()
{
    for ( int i = 0 ; i < 6 ; i++ )
        myMac[i] = 0;
}

macAddress::macAddress(QString str)
{
    setMac(str);
}

macAddress::macAddress(const macAddress &other)
{
    for (int i = 0 ; i < 6 ; i ++)
        myMac[i] = other.myMac[i];
}

QString macAddress::toString() const
{
    QString temp = QString("%1").arg( QString().setNum(myMac[0],16), 2 , QChar('0'));
    int i;
    for ( i = 1 ; i <= 5 ; i++ ) {
        temp.append(QChar(':'));
        temp = temp + QString("%1").arg(myMac[i],2,16,QChar('0'));
    }
    return temp.toUpper();
}

macAddress& macAddress::operator=(const macAddress &other)
{
    for (int i = 0 ; i < 6 ; i ++)
        myMac[i] = other.myMac[i];
    return (*this);
}

macAddress macAddress::operator++()
{
    int i = 5;
    while ( i >= 0 && myMac[i] == 255) {
        myMac[i] = 0;
        i--;
    }
    myMac[i]++;
    return *this;
}

macAddress macAddress::operator++(int notused)
{
    Q_UNUSED(notused);
    macAddress temp = *this;
    int i = 5;
    while ( i >= 0 && myMac[i] == 255) {
        myMac[i] = 0;
        i--;
    }
    myMac[i]++;
    return temp;
}

void macAddress::setBroadcast()
{
    int i;
    for (i = 0 ; i < 6 ; i++)
        myMac[i] = 0xFF;
}

bool macAddress::isBroadcast()
{
    int i;
    for (i = 0; i < 6  ; i++)
        if ( myMac[i] != 0xFF ) return false;
    return true;
}

void macAddress::setRandom()
{
    int i;
    myMac[0] = 0x01;
    for ( i = 1 ; i < 6 ; i++ )
        myMac[i] = qrand()%256;
}

void macAddress::setMac(QString str)
{
    QStringList s = str.split(":");
    for (int i = 0 ; i < 6 ; i++)
        myMac[i] = s.at(i).toInt(0,16);
}

bool macAddress::isEmpty() const
{
    for ( int i = 0 ; i < 6 ; i++ )
        if ( myMac[i] ) return false;
    return true;
}


