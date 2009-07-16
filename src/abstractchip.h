#ifndef ABSTRACTCHIP_H
#define ABSTRACTCHIP_H

#include "macaddress.h"
#include "ipaddress.h"
#include "frame.h"

class devicePort;
class ipPacket;
class deviceImpl;

/*!
  Класс являеться абстрактным устройством приема и обработки кадров.
  Являеться предком для interface и boxDevice этот класс уже дает возможность вести статистику
  входящих кадров и пакетов, поддерживает обработку и передачу кадров на уровень выше.
*/
class abstractChip
{
public:
    abstractChip();

    macAddress mac() const { return myMac; }
    ipAddress ip() const { return myIp; }
    ipAddress mask() const { return myMask; }
    void setMac(const macAddress &m) { myMac = m; }
    void setIp(const QString str) { myIp.setIp(str); }
    void setMask(const QString str) { myMask.setIp(str); }

    QString macString() { return myMac.macString(); }

    virtual void receiveEvent( frame *f ,devicePort *sender) = 0;

    quint64 countRecFrame() { return myReceiveFrame; }
    quint64 countRecPacket() { return myReceivePacket; }
    quint64 countSendFrame() { return mySendFrame; }
    quint64 countSendPacket() { return mySendPacket; }
    void addSend(int f, int p) { mySendFrame += f ; mySendPacket += p; }
    void addRec(int f, int p) { myReceiveFrame += f ; myReceivePacket += p; }

    void resetStatics();
    virtual void sendPacket(ipPacket *p, ipAddress gw = ipAddress("0.0.0.0") )
                            { Q_UNUSED(p); Q_UNUSED(gw); }
    virtual void updateArp(int u) { Q_UNUSED(u); }
    virtual void clearArp() { }
protected:
    deviceImpl *myOwner;
    ipAddress myIp;
    ipAddress myMask;
    quint64 myReceiveFrame;
    quint64 myReceivePacket;
    quint64 mySendFrame;
    quint64 mySendPacket;
    macAddress myMac;
    friend QDataStream& operator<<(QDataStream &stream,const abstractChip &chip);
    friend QDataStream& operator>>(QDataStream &stream,abstractChip &chip);
};

inline QDataStream& operator<<(QDataStream &stream,const abstractChip &chip)
{
    stream << chip.myMac;
    stream << chip.myIp;
    stream << chip.myMask;
    stream << chip.myReceiveFrame;
    stream << chip.mySendFrame;
    stream << chip.myReceivePacket;
    stream << chip.mySendPacket;
    return stream;
}

inline QDataStream& operator>>(QDataStream &stream,abstractChip &chip)
{
    stream >> chip.myMac;
    stream >> chip.myIp;
    stream >> chip.myMask;
    stream >> chip.myReceiveFrame;
    stream >> chip.mySendFrame;
    stream >> chip.myReceivePacket;
    stream >> chip.mySendPacket;
    return stream;
}

#endif // ABSTRACTCHIP_H
