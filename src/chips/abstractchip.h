#ifndef ABSTRACTCHIP_H
#define ABSTRACTCHIP_H

#ifndef __TESTING__
#include "macaddress.h"
#include "ipaddress.h"
#else
#include <QDataStream>
#endif

class frame;
class devicePort;
class ipPacket;

/*!
  Класс являеться абстрактным устройством приема и обработки кадров.
  Являеться предком для interface и boxDevice этот класс уже дает возможность вести статистику
  входящих кадров и пакетов, поддерживает обработку и передачу кадров на уровень выше.
*/
class abstractChip
{
public:
    abstractChip();
    virtual ~abstractChip() { }

    QString staticsString() const;
#ifndef __TESTING__
    void checkReceive(frame &f);
    void checkSender(frame &f);
    macAddress mac() const { return myMac; }
    ipAddress ip() const { return myIp; }
    ipAddress mask() const { return myMask; }
    virtual void receiveEvent(frame &fr,devicePort *sender) = 0;
    void setMac(const macAddress &m) { myMac = m; }
    void setIp(const QString str) { if ( !str.isEmpty() ) myIp.setIp(str); }
    void setMask(const QString str) { if ( !str.isEmpty() ) myMask.setIp(str); }
    virtual void write(QDataStream &stream) const;
    virtual void read(QDataStream &stream);
#endif
    quint64 countRecFrame() { return myReceiveFrame; }
    quint64 countRecPacket() { return myReceivePacket; }
    quint64 countSendFrame() { return mySendFrame; }
    quint64 countSendPacket() { return mySendPacket; }
    void resetStatics();
protected:
    quint64 myReceiveFrame;
    quint64 myReceivePacket;
    quint64 mySendFrame;
    quint64 mySendPacket;
#ifndef __TESTING__
    ipAddress myIp;
    ipAddress myMask;
    macAddress myMac;
#endif
};


#endif // ABSTRACTCHIP_H
