#ifndef INTERFACE_H
#define INTERFACE_H

#include <QMultiMap>
#include <QQueue>

#include "abstractchip.h"
#include "ippacket.h"

class frame;
struct arpRecord;

class interface : public abstractChip
{
public:
    enum { ethernet100 = 0 , ethernet1000 = 1 , staticMode = 100 , dinamicMode = 101 };
    interface();
    ~interface();
    void receiveEvent(frame *fr,devicePort *sender);
    void receiveIp(const ipPacket &ip);
    void receiveArp(const arpPacket &arp);
    void sendPacket(ipPacket *p,ipAddress gw = ipAddress("0.0.0.0"));
    void sendBroadcast(ipPacket *p);
    void updateArp(int u);
    void clearArp();
    devicePort* socket() { return mySocket; }
    bool isConnect() const { return mySocket->isConnect(); }

    ipPacket popPacket() { return buffer.dequeue(); }
    arpRecord* addToTable( ipAddress ip , macAddress mac , int mode );
    void removeFromTable (QString ip);
    frame createFrame( macAddress senderMac, macAddress receiverMac , int t);
    QList<arpRecord*> arpTable() const { return myArpTable; }

    void setName(const QString &str) { myName = str; }
    QString name() const { return myName; }
private:
    QString myName;
    QQueue<ipPacket> buffer; //!< Очередь входящих ip-пакетов.
    devicePort *mySocket;
    QList<arpRecord*> myArpTable;
    QMultiMap<ipAddress,ipPacket*> myWaits;
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
