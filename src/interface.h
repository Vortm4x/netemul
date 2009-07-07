#ifndef INTERFACE_H
#define INTERFACE_H

#include <QMultiMap>

#include "abstractchip.h"
#include "ippacket.h"


class smartDevice;
class frame;

struct arpRecord;

const QString names[] = { QObject::trUtf8("Ethernet(10/100 Мбит/с)") , QObject::trUtf8("Ethernet(10/100/1000 Мбит/с)") };


class interface : public abstractChip
{
    Q_OBJECT
public:
    enum { ethernet100 = 0 , ethernet1000 = 1 , staticMode = 100 , dinamicMode = 101 };
    interface(devicePort *parent,  int t = 0 );
    ~interface();
    QString nameInterface() { return names[myType]; }
    void receiveEvent(frame *fr,devicePort *sender);
    void receiveIp( ipPacket *ip , devicePort *sender);
    void receiveArp( arpPacket arp , devicePort *sender);
    void sendPacket(ipPacket *p,ipAddress gw = ipAddress("0.0.0.0"));
    void sendBroadcast(ipPacket *p);
    void setSmart(smartDevice *s) { mySmart = s; }
    void updateArp(int u);
    void clearArp();
    smartDevice* smart() { return mySmart; }
    arpRecord* addToTable( ipAddress ip , macAddress mac , int mode );
    void removeFromTable (QString ip);
    frame* createFrame( macAddress senderMac, macAddress receiverMac , int t);
    QList<arpRecord*> arpTable() const { return myArpTable; }
private:
    smartDevice *mySmart;
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
