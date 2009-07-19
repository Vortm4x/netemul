#ifndef HUBDEVICE_H
#define HUBDEVICE_H

#include "boxdevice.h"
#include "hubchip.h"

class frame;
class hubSetting;

class hubDevice : public boxDevice
{
public:
    friend class hubSetting;
    int type() const { return hubDev; }
    enum { hubDev = 4 };
    hubDevice(int c = 4);
    ~hubDevice();
    devicePort* addInterface(QString str,int t);
    void dialog();
protected:
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
};

class hubSetting {
public:
    hubSetting(hubDevice *h) { hub = h; }
    int socketsCount() const { return hub->chip->socketsCount(); }
    QString snmpMac() const { return hub->chip->mac().toString(); }
    QString snmpIp() const { return hub->chip->ip().toString(); }
    QString snmpMask() const { return hub->chip->mask().toString(); }
    void setMac(const QString &str) { hub->chip->setMac(str); }
    void setIp(const QString &str) { hub->chip->setIp(str); }
    void setMask(const QString &str) { hub->chip->setMask(str); }
    QString statics() const { return hub->chip->staticsString(); }
    QString note() const { return hub->note(); }
    void setNote(const QString &str) { hub->setNote(str); }
    bool isManual() const { return hub->isManual(); }
    void setManual(bool b) { hub->setManual(b); }
    bool setSocketsCount(int n) { return hub->chip->setSocketsCount(n); }
private:
    hubDevice *hub;
};



#endif // HUBDEVICE_H
