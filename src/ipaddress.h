#ifndef IPADDRESS_H
#define IPADDRESS_H

#include <QString>
#include <QDataStream>
#include <QtDebug>

class ipAddress
{
public:
    ipAddress();
    ipAddress(const quint8  *cur);
    ipAddress(const QString str);
    void setIp(const quint8 *cur);
    void setIp(const QString str);
    QString ipString() const;
    bool isEmpty() const;
    unsigned int toInt() const;
    quint8 at(int i) const { return myIp[i]; }
    ipAddress operator=(ipAddress other);
    ipAddress operator&(const ipAddress &e1) const;
    ipAddress operator~() const;
    ipAddress operator|(const ipAddress &e1) const;
    static ipAddress full() { return ipAddress("255.255.255.255"); }
private:
    quint8 myIp[4];
protected:
    friend inline bool operator<(const ipAddress &e1 , const ipAddress &e2);
    friend inline bool operator>(const ipAddress &e1 , const ipAddress &e2);
    friend QDataStream& operator>>(QDataStream &stream, ipAddress &address);
    friend QDataStream& operator<<(QDataStream &stream, const ipAddress &address);
    friend QDebug operator<<(QDebug dbg, const ipAddress &c);
};

inline bool operator<(const ipAddress &e1 , const ipAddress &e2) { return e1.toInt() < e2.toInt(); }
inline bool operator>(const ipAddress &e1 , const ipAddress &e2) { return e1.toInt() > e2.toInt(); }
inline bool operator<=(const ipAddress &e1 , const ipAddress &e2) { return !(e1.toInt() > e2.toInt()); }
inline bool operator>=(const ipAddress &e1 , const ipAddress &e2) { return !(e1.toInt() < e2.toInt()); }
inline bool operator==(const ipAddress &e1 , const ipAddress &e2) { return e1.toInt() == e2.toInt(); }
inline bool operator!=(const ipAddress &e1, const ipAddress &e2) { return !(e1 == e2) ; }

QDataStream& operator>>(QDataStream &stream, ipAddress &address);
QDataStream& operator<<(QDataStream &stream, const ipAddress &address);
QDebug operator<<(QDebug dbg, const ipAddress &c);

#endif // IPADDRESS_H
