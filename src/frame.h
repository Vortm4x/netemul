#ifndef FRAME_H
#define FRAME_H

#include "macaddress.h"
#include <QBrush>
#include <QString>
#include "ippacket.h"
#include "arppacket.h"

class frame
{
public:
    enum direction { startToEnd = 0 , endToStart = 1 , tArp = 100 , tIp = 101 };
    void setPos(qreal temp) { myPos = temp; }
    qreal pos() { return myPos; }
    direction direct() { return myDirect; }
    void setDirection(direction temp) { myDirect = temp; }
    frame();
    frame(const frame &other);
    ~frame();
    frame operator=(frame other);
    macAddress sender() const { return mySender; }
    void setSender(macAddress temp) { mySender = temp; }
    macAddress receiver() const { return myReceiver; }
    void setReceiver(macAddress temp) { myReceiver = temp; if ( temp.isBroadcast() ) setColor(Qt::yellow);
                                                           else setColor( Qt::red );  }
    QBrush& color() { return myColor; }
    void setColor(QBrush b) { myColor = b; }
    int type() const { return myType; }
    void setType(int t) { myType = t; }
    void setPacket(ipPacket *p) { if (myPacket) delete myPacket; myPacket = p; }
    ipPacket* packet() const { return myPacket;}
    void setArp(arpPacket *p) { if (myArp) delete myArp; myArp = p; }
    arpPacket* arp() const { return myArp; }
private:
    macAddress mySender;
    macAddress myReceiver;
    QBrush myColor;
    qreal myPos;
    direction myDirect;
    int myType;
    ipPacket *myPacket;
    arpPacket *myArp;
};

#endif // FRAME_H
