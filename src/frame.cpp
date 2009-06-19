#include "frame.h"
#include "macaddress.h"
#include <QDataStream>
#include <QtDebug>

frame::frame()
{
    myColor = Qt::red;
    myPacket = NULL;
    myArp = NULL;
}

frame::~frame()
{
    if ( myPacket ) delete myPacket;
    if ( myArp ) delete myArp;
}

frame frame::operator=(frame other)
{
    myDirect = other.myDirect;
    myPos = other.myPos;
    mySender = other.mySender;
    myReceiver = other.myReceiver;
    myColor = other.myColor;
    myType = other.myType;
    if ( myType == tIp ) {
        if ( myPacket ) delete myPacket;
        if ( other.myPacket ) {
            myPacket = new ipPacket;
            *myPacket = *(other.myPacket);
        } else myPacket = NULL;
    }
    else {
        if ( myArp ) delete myArp;
        if ( other.myArp ) {
            myArp = new arpPacket;
            *myArp = *(other.myArp);
        } else myArp = NULL;
    }
    return *this;
}

frame::frame(const frame &other)
{
    myColor = Qt::red;
    myPacket = NULL;
    myArp = NULL;
    myDirect = other.myDirect;
    myPos = other.myPos;
    mySender = other.mySender;
    myReceiver = other.myReceiver;
    myColor = other.myColor;
    myType = other.myType;
    if ( myType == tIp ) {
        if ( other.myPacket ) {
            myPacket = new ipPacket;
            *myPacket = *(other.myPacket);
        }
    }
    else {
        if ( other.myArp ) {
            myArp = new arpPacket;
            *myArp = *(other.myArp);
        }
    }
}
