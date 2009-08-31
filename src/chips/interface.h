#ifndef INTERFACE_H
#define INTERFACE_H

#include "abstractchip.h"
#include "ippacket.h"
#include "arppacket.h"

class cableDev;
class arpModel;

static const quint8 COUNT_AGAINST_SEND = 5;

struct waitPacket
{
    ipAddress dest;
    int time;
    quint8 count;
    QList<ipPacket> packets;
    void insert(ipPacket p) { packets << p; }
    static waitPacket* create(ipAddress a,ipPacket p);
};

class interface : public abstractChip
{
    Q_OBJECT
public:
    interface(const QString &name);
    interface() { }
    ~interface();
    void receiveEvent(frame &fr,devicePort*);
    void receiveIp(ipPacket &ip);
    void receiveArp(arpPacket &arp);
    void sendPacket(ipPacket &p,ipAddress gw = ipAddress("0.0.0.0"));
    void sendBroadcast(ipPacket &p);
    const devicePort* socket() const { return mySocket; }
    bool isConnect() const;
    void setConnect(bool b,cableDev *c);
    bool isCableConnect(const cableDev *c) const;
    void deciSecondEvent();
    void secondEvent();
    void sendArpRequest(ipAddress a);
    void sendArpResponse(macAddress m, ipAddress a);
    int trafficDigit() const;
    bool isBusy() const;

    frame createFrame( macAddress receiverMac , int t);
    void setChecked(bool b);
    arpModel* arpTable() const { return myArpTable; }

    virtual void write(QDataStream &stream) const;
    virtual void read(QDataStream &stream);
    void setName(const QString &str) { myName = str; }
    QString name() const { return myName; }
signals:
    void receivedPacket(ipPacket);
private:
    void pushToSocket(frame &f);
    QString myName;
    devicePort *mySocket;
    arpModel *myArpTable;
    QList<waitPacket*> myWaits;
};

#endif // INTERFACE_H
