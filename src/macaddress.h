#ifndef MACADDRESS_H
#define MACADDRESS_H

#include <QDataStream>

class macAddress
{
public:
    macAddress();
    macAddress(QString str);
    macAddress(const macAddress &other);
    static QString random() { macAddress m; m.setRandom(); return m.macString(); }
    void setMac(QString str);
    void setRandom();
    quint8 at(int i) const { return myMac[i]; }
    QString macString() const ;
    macAddress operator++();
    macAddress operator++(int notused);
    macAddress& operator=(const macAddress &other);
    void setBroadcast();
    bool isBroadcast();
private:
    quint8 myMac[6];
protected:
    friend QDataStream& operator<<(QDataStream &stream, const macAddress &address);
    friend QDataStream& operator>>(QDataStream &stream, macAddress &address);
};

inline bool operator<(const macAddress &e1 , const macAddress &e2) { return e1.macString() < e2.macString(); }
inline bool operator>(const macAddress &e1 , const macAddress &e2) { return e1.macString() > e2.macString(); }
inline bool operator==(const macAddress &e1 , const macAddress &e2) { return e1.macString() == e2.macString(); }
inline bool operator!=(const macAddress &e1, const macAddress &e2) { return !(e1 == e2) ; }
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
