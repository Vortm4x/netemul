#ifndef ABSTRACTSOCKET_H
#define ABSTRACTSOCKET_H

#include "ippacket.h"

class smartDevice;

class abstractSocket : public QObject
{
    Q_OBJECT
public:
    abstractSocket() { dev = 0; }
    abstractSocket(smartDevice *d);
    virtual ~abstractSocket();
    bool isOurData(ipAddress address, quint16 port);
    void setBind(ipAddress address) { myBind = address; }
    virtual void treatPacket(ipPacket) { }
    virtual void secondEvent() { }
    virtual void write(ipAddress, quint16, QByteArray) { }
signals:
    void readyRead(QByteArray);
protected:
    quint16 myBindPort;
    ipAddress myBind;
    smartDevice *dev;
};

#endif // ABSTRACTSOCKET_H
