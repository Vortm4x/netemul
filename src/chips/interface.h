#ifndef INTERFACE_H
#define INTERFACE_H

#include <QMultiMap>
#include <QQueue>

#include "abstractchip.h"
#include "ippacket.h"
#include "arppacket.h"

class frame;
struct arpRecord;
class cableDev;
class arpModel;

class interface : public abstractChip
{
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
    bool isBusy() const;

    ipPacket popPacket() { return buffer.dequeue(); }    
    frame createFrame( macAddress receiverMac , int t);
    bool hasReceive() const { return !buffer.isEmpty(); }
    ipPacket popFromReceive() { return buffer.dequeue(); }
    void setChecked(bool b);
    arpModel* arpTable() const { return myArpTable; }

    virtual void write(QDataStream &stream) const;
    virtual void read(QDataStream &stream);
    void setName(const QString &str) { myName = str; }
    QString name() const { return myName; }
private:
    void pushToSocket(frame &f);
    QString myName;
    QQueue<ipPacket> buffer; //!< Очередь входящих ip-пакетов.
    devicePort *mySocket;
    arpModel *myArpTable;
    QMultiMap<ipAddress,ipPacket> myWaits;
};

#endif // INTERFACE_H
