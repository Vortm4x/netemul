#ifndef IPPACKET_H
#define IPPACKET_H

#include "ipaddress.h"
#include "udppacket.h"
#include "tcppacket.h"

class ipPacket
{
public:
    /*! Используется для обозначения протокола верхнего уровня. */
    enum { udp = 0 , tcp = 1 };
    ipPacket();
    ipPacket(ipAddress s,ipAddress r) { mySender = s ; myReceiver = r; }
    ipAddress sender() const { return mySender; }
    ipAddress receiver() const { return myReceiver; }
    void setSender(ipAddress a) { mySender = a; }
    void setReceiver(ipAddress a) { myReceiver = a; }
    bool isBroadcast(const ipAddress mask) const;
    bool isBroadcast(const QString str) const { return isBroadcast(ipAddress(str)); }
    void setBroadcast(const ipAddress mask);
    void setBroadcast(const QString str) { setBroadcast(ipAddress(str)); }
    ipPacket operator=(ipPacket other);
    /*! Задать протокол верхнего уровня
        @param u - идентификатор протокола(константы tcp или udp) */
    void setUpProtocol(qint8 u) { myUpProtocol = u; }
    /*!  @return идентификатор протокола верхнего уровня. */
    qint8 upProtocol() const { return myUpProtocol; }
    void operator<<(tcpPacket &p);
    void operator>>(tcpPacket &p) const;
    void operator<<(udpPacket &p);
    void operator>>(udpPacket &p) const;
private:
    ipAddress mySender; //!< Адрес отправителя.
    ipAddress myReceiver; //!< Адрес получателя.
    qint8 myUpProtocol; //!< Протокол верхнего уровня
    QByteArray data; //!< Данные протокола более высокого уровня.
protected:
    friend QDataStream& operator<<(QDataStream &stream,const ipPacket &p);
    friend QDataStream& operator>>(QDataStream &stream,ipPacket &p);
};

#endif // IPPACKET_H
