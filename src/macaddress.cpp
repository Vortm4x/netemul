#include "macaddress.h"
#include <QStringList>

macAddress::macAddress()
{
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

QString macAddress::macString() const
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



