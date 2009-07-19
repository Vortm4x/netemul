#ifndef MACADDRESS_H
#define MACADDRESS_H

#include <QDataStream>

class macAddress
{
public:
    macAddress();
    macAddress(QString str);
    macAddress(const macAddress &other);
    void setMac(QString str);
    void setRandom();
    QString toString() const ;
    macAddress operator++();
    macAddress operator++(int notused);
    macAddress& operator=(const macAddress &other);
    void setBroadcast();
    bool isBroadcast();
    qulonglong toInt() const;
private:
    quint8 myMac[6];
protected:
    friend QDataStream& operator<<(QDataStream &stream, const macAddress &address);
    friend QDataStream& operator>>(QDataStream &stream, macAddress &address);
};

inline qulonglong macAddress::toInt() const
{
    return ((qulonglong)myMac[0]<<40)+((qulonglong)myMac[1]<<32)+((qulonglong)myMac[2]<<24)+
            ((qulonglong)myMac[3]<<16)+((qulonglong)myMac[4]<<8)+(qulonglong)myMac[5];
}
inline bool operator<(const macAddress &e1 , const macAddress &e2) { return e1.toInt() < e2.toInt(); }
inline bool operator>(const macAddress &e1 , const macAddress &e2) { return e1.toInt() > e2.toInt(); }
inline bool operator==(const macAddress &e1 , const macAddress &e2) { return e1.toInt() == e2.toInt(); }
inline bool operator!=(const macAddress &e1, const macAddress &e2) { return !(e1 == e2); }
inline bool operator>=(const macAddress &e1 , const macAddress &e2) { return !(e1 < e2); }
inline bool operator<=(const macAddress &e1 , const macAddress &e2) { return !(e1 > e2); }


inline QDataStream& operator<<(QDataStream &stream, const macAddress &address)
{
    for (int i = 0 ; i < 6 ; i++ ) stream << address.myMac[i];
    return stream;
}

inline QDataStream& operator>>(QDataStream &stream, macAddress &address)
{
    for (int i = 0 ; i < 6 ; i++ ) stream >> address.myMac[i];
    return stream;
}

#endif // MACADDRESS_H
