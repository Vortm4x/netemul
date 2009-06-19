#ifndef MACADDRESS_H
#define MACADDRESS_H

#include <QString>
#include <QDataStream>

class macAddress
{
public:
    macAddress();
    macAddress(const quint8 *cur);
    macAddress(QString str);
    static QString random() { macAddress m; m.setRandom(); return m.macString(); }
    void setMac( const quint8 *cur);
    void setMac(QString str);
    void setRandom();
    quint8 at(int i) const { return myMac[i]; }
    QString macString() const ;
    const quint8* mac() const { return myMac; }
    macAddress operator++();
    macAddress operator++(int notused);
    macAddress operator=(macAddress other);
    void setBroadcast();
    bool isBroadcast();
private:
    quint8 myMac[6];
};

inline bool operator<(const macAddress &e1 , const macAddress &e2) { return e1.macString() < e2.macString(); }
inline bool operator>(const macAddress &e1 , const macAddress &e2) { return e1.macString() > e2.macString(); }
inline bool operator==(const macAddress &e1 , const macAddress &e2) { return e1.macString() == e2.macString(); }
inline bool operator!=(const macAddress &e1, const macAddress &e2) { return !(e1 == e2) ; }
inline bool operator>=(const macAddress &e1 , const macAddress &e2) { return !(e1 < e2); }
inline bool operator<=(const macAddress &e1 , const macAddress &e2) { return !(e1 > e2); }

QDataStream& operator<<(QDataStream &stream, const macAddress &address);
QDataStream& operator>>(QDataStream &stream, macAddress &address);


#endif // MACADDRESS_H
