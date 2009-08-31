#ifndef BOXDEVICE_H
#define BOXDEVICE_H

#include "device.h"
#include "boxchip.h"
#include "abstractchip.h"

class boxDevice : public deviceImpl
{
public:
    friend class boxSetting;
    boxDevice();
    virtual ~boxDevice();
    bool isConnectSocket(const QString &socket) const { return chip->isConnectSocket(socket); }
    QStringList sockets() const { return chip->sockets(); }
    QString socketName(const cableDev *c) const { return chip->socketName(c); }
    void addConnection(const QString &port , cableDev *c) { chip->addConnection(port,c); }
    void deleteConnection(cableDev *c) { chip->deleteConnection(c); }
    void setManual(bool c) { myManual = c; }
    bool isManual() { return myManual; }
    void deciSecondTimerEvent();
    int trafficDigit() const { return chip->trafficDigit(); }
    virtual void secondTimerEvent() { }
    virtual void showLogDialog(logDialog *log) const;
    bool isBusy() const { return chip->isBusy(); }
    statistics deviceStatistics() const { return chip->chipStatistics(); }
public slots:
    void setSocketsCount(int n) { chip->setSocketsCount(n); }
    void setCheckedSocket(const QString &port);
protected:
    bool myManual;
    boxChip *chip;
    virtual void write(QDataStream &stream) const;
    virtual void read(QDataStream &stream);
};

class boxSetting : public deviceSetting {
public:
    boxSetting(boxDevice *h) : deviceSetting(h) , box(h) {  }
    int socketsCount() const { return box->chip->socketsCount(); }
    QString snmpMac() const { return box->chip->mac().toString(); }
    QString snmpIp() const { return box->chip->ip().toString(); }
    QString snmpMask() const { return box->chip->mask().toString(); }
    void setMac(const QString &str) { box->chip->setMac(str); }
    void setIp(const QString &str) { box->chip->setIp(str); }
    void setMask(const QString &str) { box->chip->setMask(str); }
    QString statics() const { return box->chip->staticsString(); }
    bool isManual() const { return box->isManual(); }
    void setManual(bool b) { box->setManual(b); }
    bool setSocketsCount(int n) { return box->chip->setSocketsCount(n); }
private:
    boxDevice *box;
};

#endif // BOXDEVICE_H
