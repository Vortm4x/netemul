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
#ifndef MACADDRESS_H
#define MACADDRESS_H

#include <QDataStream>
#include "basicnetlib_global.h"

class BASICNETLIBSHARED_EXPORT MacAddress
{
public:
    MacAddress();
    MacAddress(QString str);
    MacAddress(const MacAddress &other);
    void setMac(QString str);
    void setRandom();
    QString toString() const ;
    MacAddress operator++();
    MacAddress operator++(int notused);
    MacAddress& operator=(const MacAddress &other);
    void setBroadcast();
    bool isBroadcast();
    bool isEmpty() const;
    qulonglong toInt() const;
private:
    quint8 myMac[6];
protected:
    friend QDataStream& operator<<(QDataStream &stream, const MacAddress &address);
    friend QDataStream& operator>>(QDataStream &stream, MacAddress &address);
};

inline qulonglong MacAddress::toInt() const
{
    return ((qulonglong)myMac[0]<<40)+((qulonglong)myMac[1]<<32)+((qulonglong)myMac[2]<<24)+
            ((qulonglong)myMac[3]<<16)+((qulonglong)myMac[4]<<8)+(qulonglong)myMac[5];
}
inline bool operator<(const MacAddress &e1 , const MacAddress &e2) { return e1.toInt() < e2.toInt(); }
inline bool operator>(const MacAddress &e1 , const MacAddress &e2) { return e1.toInt() > e2.toInt(); }
inline bool operator==(const MacAddress &e1 , const MacAddress &e2) { return e1.toInt() == e2.toInt(); }
inline bool operator!=(const MacAddress &e1, const MacAddress &e2) { return !(e1 == e2); }
inline bool operator>=(const MacAddress &e1 , const MacAddress &e2) { return !(e1 < e2); }
inline bool operator<=(const MacAddress &e1 , const MacAddress &e2) { return !(e1 > e2); }


inline QDataStream& operator<<(QDataStream &stream, const MacAddress &address)
{
    for (int i = 0 ; i < 6 ; i++ ) stream << address.myMac[i];
    return stream;
}

inline QDataStream& operator>>(QDataStream &stream, MacAddress &address)
{
    for (int i = 0 ; i < 6 ; i++ ) stream >> address.myMac[i];
    return stream;
}

#endif // MACADDRESS_H

