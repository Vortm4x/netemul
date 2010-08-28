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
#ifndef IPADDRESS_H
#define IPADDRESS_H

#include <QDataStream>
#include <QVariant>

class IpAddress
{
public:
    IpAddress();
    IpAddress(const quint8  *cur);
    IpAddress(const QString str);
    IpAddress(const char *str);
    IpAddress(const IpAddress &other);
    IpAddress(const quint32 intIp);
    void setIp(const quint8 *cur);
    void setIp(const QString str);
    QString toString() const;
    bool isEmpty() const;
    bool isLoopBack() const { return myIp[0] == 127; }
    bool isFull() const;
    quint32 toInt() const;
    quint8 at(int i) const { return myIp[i]; }
    IpAddress& operator=(const IpAddress &other);
    IpAddress operator&(const IpAddress &e1) const;
    IpAddress operator~() const;
    IpAddress operator|(const IpAddress &e1) const;
    static IpAddress full() { return IpAddress("255.255.255.255"); }
    void writeXml() const { }
private:
    quint8 myIp[4];
protected:
    friend QDataStream& operator>>(QDataStream &stream, IpAddress &address);
    friend QDataStream& operator<<(QDataStream &stream, const IpAddress &address);
};

inline quint32 IpAddress::toInt() const
{
    return ( (myIp[0] << 24) + (myIp[1] << 16) + (myIp[2] << 8) +myIp[3]);
}

inline bool operator<(const IpAddress &e1 , const IpAddress &e2) { return e1.toInt() < e2.toInt(); }
inline bool operator>(const IpAddress &e1 , const IpAddress &e2) { return e1.toInt() > e2.toInt(); }
inline bool operator<=(const IpAddress &e1 , const IpAddress &e2) { return !(e1.toInt() > e2.toInt()); }
inline bool operator>=(const IpAddress &e1 , const IpAddress &e2) { return !(e1.toInt() < e2.toInt()); }
inline bool operator==(const IpAddress &e1 , const IpAddress &e2) { return e1.toInt() == e2.toInt(); }
inline bool operator!=(const IpAddress &e1, const IpAddress &e2) { return !(e1 == e2) ; }
inline QDataStream& operator<<(QDataStream &stream, const IpAddress &address)
{
    for (int i = 0 ; i < 4 ; i++ ) stream << address.myIp[i];
    return stream;
}
inline QDataStream& operator>>(QDataStream &stream, IpAddress &address)
{
    for (int i = 0 ; i < 4 ; i++ ) stream >> address.myIp[i];
    return stream;
}

#endif // IPADDRESS_H
