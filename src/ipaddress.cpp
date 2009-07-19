#include "ipaddress.h"
#include <QStringList>

ipAddress::ipAddress()
{
    for ( int i = 0 ; i < 4 ; i++ ) myIp[i] = 0;
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

ipAddress::ipAddress(const ipAddress &other)
{
    for ( int i = 0; i < 4 ; i++)
        myIp[i] = other.myIp[i];
}

QString ipAddress::toString() const
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

ipAddress& ipAddress::operator=(const ipAddress &other)
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

bool ipAddress::isEmpty() const
{
    int i,d = 0;
    for ( i = 0 ; i < 4 ; i++ ) d += myIp[i];
    return !d;
}

ipAddress ipAddress::operator ~() const
{
    ipAddress ip;
    for ( int i = 0; i < 4 ; i++)
        ip.myIp[i] = ~myIp[i];
    return ip;
}

