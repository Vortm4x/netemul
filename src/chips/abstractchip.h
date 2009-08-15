#ifndef ABSTRACTCHIP_H
#define ABSTRACTCHIP_H

#include "statistics.h"

#ifndef __TESTING__
#include "macaddress.h"
#include "ipaddress.h"
#include "frame.h"
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
class abstractChip : public QObject
{
    Q_OBJECT
public:
    abstractChip();
    virtual ~abstractChip() { }
    virtual void receiveEvent(frame &fr,devicePort *sender) = 0;
    QString staticsString() const { return myStatistics.toString(); }
    void checkReceive(frame &f);
    void checkSend(frame &f);
#ifndef __TESTING__
    macAddress mac() const { return myMac; }
    ipAddress ip() const { return myIp; }
    ipAddress mask() const { return myMask; }
    void setMac(const macAddress &m) { myMac = m; }
    void setIp(const QString str) { if ( !str.isEmpty() ) myIp.setIp(str); }
    void setMask(const QString str) { if ( !str.isEmpty() ) myMask.setIp(str); }
    virtual void write(QDataStream &stream) const;
    virtual void read(QDataStream &stream);
    statistics chipStatistics() { return myStatistics; }
    quint64 countRecFrame() { return myStatistics.receiveFrames(); }
    quint64 countRecPacket() { return myStatistics.receivePackets(); }
    quint64 countSendFrame() { return myStatistics.sendFrames(); }
    quint64 countSendPacket() { return myStatistics.sendPackets(); }
#endif
    void resetStatics();
signals:
    void sendData(frame,QString);
    void receiveData(frame,QString);
protected:
    statistics myStatistics;
#ifndef __TESTING__
    ipAddress myIp;
    ipAddress myMask;
    macAddress myMac;
#endif
};


#endif // ABSTRACTCHIP_H
