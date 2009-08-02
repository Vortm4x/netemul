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
    enum { ethernet100 = 0 , ethernet1000 = 1 , staticMode = 100 , dinamicMode = 101 };
    interface(const QString &name);
    interface() { }
    ~interface();
    void receiveEvent(frame &fr,devicePort*);
    void receiveIp(ipPacket &ip);
    void receiveArp(arpPacket &arp);
    void sendPacket(ipPacket &p,ipAddress gw = ipAddress("0.0.0.0"));
    void sendBroadcast(ipPacket &p);
    void updateArp();
    void clearArp();
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
    arpRecord* addToTable( ipAddress ip , macAddress mac , int mode );
    void deleteFromTable(const QString &ip);
    void deleteFromTable(arpRecord *r);
    frame createFrame( macAddress receiverMac , int t);
    bool hasReceive() const { return !buffer.isEmpty(); }
    ipPacket popFromReceive() { return buffer.dequeue(); }
    void setChecked(bool b);
    //arpModel* arpTable() const { return myArpTable; }

    virtual void write(QDataStream &stream) const;
    virtual void read(QDataStream &stream);
    void setName(const QString &str) { myName = str; }
    QString name() const { return myName; }
private:
    void pushToSocket(frame &f);
    QString myName;
    QQueue<ipPacket> buffer; //!< Очередь входящих ip-пакетов.
    devicePort *mySocket;
    QList<arpRecord*> myArpTable;
    //arpModel *myArpTable;
    QMultiMap<ipAddress,ipPacket> myWaits;
};

struct arpRecord {
    macAddress mac;
    ipAddress ip;
    int time;
    int mode;
    QString modeString() const {
        if ( mode == interface::staticMode ) return QObject::trUtf8("Статическая");
        else return QObject::trUtf8("Динамическая");
    }
};

#endif // INTERFACE_H
