#include "frame.h"

frame::frame()
{
    myDifferent = normal;
}

frame::~frame()
{

}

frame::frame(const frame &other)
{
    mySender = other.mySender;
    myReceiver = other.myReceiver;
    myType = other.myType;
    myDifferent = other.myDifferent;
    data = other.data;
}

frame frame::operator=(frame &other)
{
    mySender = other.mySender;
    myReceiver = other.myReceiver;
    myType = other.myType;
    myDifferent = other.myDifferent;
    data = other.data;
    return *this;
}


