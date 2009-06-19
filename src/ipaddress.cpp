#include "ipaddress.h"
#include <QStringList>

ipAddress::ipAddress()
{
}

ipAddress::ipAddress(const quint8 *cur)
{
    for ( int i = 0; i < 4 ; i++)
        myIp[i] = cur[i];
}

ipAddress::ipAddress(const QString str)
{
    QStringList s = str.split(".");
    for (int i = 0 ; i < 4 ; i++)
        myIp[i] = s.at(i).toInt();
}

QString ipAddress::ipString() const
{
    QString temp = QString().setNum(myIp[0]);
    for ( int i = 1 ; i <= 3 ; i++ )
        temp += QChar('.') + QString().setNum(myIp[i]);
    return temp;
}

void ipAddress::setIp(const quint8 *cur)
{
    for ( int i = 0; i < 4 ; i++)
        myIp[i] = cur[i];
}

ipAddress ipAddress::operator=(ipAddress other)
{
    for ( int i = 0; i < 4 ; i++)
        myIp[i] = other.myIp[i];
    return (*this);
}

void ipAddress::setIp(const QString str)
{
    QStringList s = str.split(".");
    for (int i = 0 ; i < 4 ; i++)
        myIp[i] = s.at(i).toInt();
}

QDataStream& operator<<(QDataStream &stream, const ipAddress &address)
{
    int i;
    for ( i = 0 ; i < 4 ; i++ ) stream << address.myIp[i];
    return stream;
}

QDataStream& operator>>(QDataStream &stream, ipAddress &address)
{
    int i;
    quint8 t[4];
    for ( i = 0 ; i < 4 ; i++ ) stream >> t[i];
    address.setIp(t);
    return stream;
}

ipAddress ipAddress::operator&(const ipAddress &e1) const
{
    int i;
    ipAddress e3;
    for (i=0; i < 4; i++)
        e3.myIp[i] = this->myIp[i] & e1.myIp[i];
    return e3;
}

ipAddress ipAddress::operator |(const ipAddress &e1) const
{
    int i;
    ipAddress e3;
    for (i=0; i < 4; i++)
        e3.myIp[i] = this->myIp[i] | e1.myIp[i];
    return e3;
}

unsigned int ipAddress::toInt() const
{
    unsigned int d = 0;
    for ( int i = 0 ; i < 4 ; i++ )
        d += myIp[i] << (8*(3-i));
    return d;
}

bool ipAddress::isEmpty() const
{
    int i,d = 0;
    for ( i = 0 ; i < 4 ; i++ ) d += myIp[i];
    return !d;
}

QDebug operator<<(QDebug dbg, const ipAddress &c)
{
    for (int i = 0 ; i < 4 ; i++ ) {
     dbg.nospace() << c.myIp[i];
     if ( i < 3 ) dbg.nospace() << '.';
    }
    return dbg.space();
}

ipAddress ipAddress::operator ~() const
{
    ipAddress ip;
    for ( int i = 0; i < 4 ; i++)
        ip.myIp[i] = ~myIp[i];
    return ip;
}
