#ifndef IPADDRESS_H
#define IPADDRESS_H

#include <QDataStream>

class ipAddress
{
public:
    ipAddress();
    ipAddress(const quint8  *cur);
    ipAddress(const QString str);
    ipAddress(const ipAddress &other);
    void setIp(const quint8 *cur);
    void setIp(const QString str);
    QString toString() const;
    bool isEmpty() const;
    bool isLoopBack() const { return myIp[0] == 127; }
    unsigned int toInt() const;
    quint8 at(int i) const { return myIp[i]; }
    ipAddress& operator=(const ipAddress &other);
    ipAddress operator&(const ipAddress &e1) const;
    ipAddress operator~() const;
    ipAddress operator|(const ipAddress &e1) const;
    static ipAddress full() { return ipAddress("255.255.255.255"); }
private:
    quint8 myIp[4];
protected:
    friend QDataStream& operator>>(QDataStream &stream, ipAddress &address);
    friend QDataStream& operator<<(QDataStream &stream, const ipAddress &address);
};
inline unsigned int ipAddress::toInt() const
{
    return ( (myIp[0] << 24) + (myIp[1] << 16) + (myIp[2] << 8) +myIp[3]);
}
inline bool operator<(const ipAddress &e1 , const ipAddress &e2) { return e1.toInt() < e2.toInt(); }
inline bool operator>(const ipAddress &e1 , const ipAddress &e2) { return e1.toInt() > e2.toInt(); }
inline bool operator<=(const ipAddress &e1 , const ipAddress &e2) { return !(e1.toInt() > e2.toInt()); }
inline bool operator>=(const ipAddress &e1 , const ipAddress &e2) { return !(e1.toInt() < e2.toInt()); }
inline bool operator==(const ipAddress &e1 , const ipAddress &e2) { return e1.toInt() == e2.toInt(); }
inline bool operator!=(const ipAddress &e1, const ipAddress &e2) { return !(e1 == e2) ; }
inline QDataStream& operator<<(QDataStream &stream, const ipAddress &address)
{
    for (int i = 0 ; i < 4 ; i++ ) stream << address.myIp[i];
    return stream;
}
inline QDataStream& operator>>(QDataStream &stream, ipAddress &address)
{
    for (int i = 0 ; i < 4 ; i++ ) stream >> address.myIp[i];
    return stream;
}

#endif // IPADDRESS_H
