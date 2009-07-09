#include "frame.h"

frame::frame()
{

}

frame::~frame()
{

}

frame::frame(const frame &other)
{
    mySender = other.mySender;
    myReceiver = other.myReceiver;
    myType = other.myType;
    data = other.data;
}

frame frame::operator=(frame &other)
{
    mySender = other.mySender;
    myReceiver = other.myReceiver;
    myType = other.myType;
    data = other.data;
    return *this;
}


