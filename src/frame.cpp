#include "frame.h"
#include <QDataStream>

frame::frame()
{
    myColor = Qt::red;
}

frame::~frame()
{

}

frame::frame(const frame &other)
{
    myDirect = other.myDirect;
    myPos = other.myPos;
    mySender = other.mySender;
    myReceiver = other.myReceiver;
    myColor = other.myColor;
    myType = other.myType;
    data = other.data;
}

frame frame::operator=(frame &other)
{
    myDirect = other.myDirect;
    myPos = other.myPos;
    mySender = other.mySender;
    myReceiver = other.myReceiver;
    myColor = other.myColor;
    myType = other.myType;
    data = other.data;
    return *this;
}


