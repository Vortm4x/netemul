#include "frame.h"

frame::frame()
{
    myDifferent = normal;
}

frame::~frame()
{

}

frame::frame(const QByteArray &b)
{
    QDataStream s(b);
    s >> myDifferent >> mySender >> myReceiver >> myType  >> data;
}

frame::frame(const frame &other)
{
    mySender = other.mySender;
    myReceiver = other.myReceiver;
    myType = other.myType;
    myDifferent = other.myDifferent;
    data = other.data;
}

frame& frame::operator=(const frame &other)
{
    mySender = other.mySender;
    myReceiver = other.myReceiver;
    myType = other.myType;
    myDifferent = other.myDifferent;
    data = other.data;
    return *this;
}

QByteArray frame::toData() const
{
    QByteArray t;
    QDataStream s(&t,QIODevice::WriteOnly);
    s << myDifferent << mySender << myReceiver << myType << data;
    return t;
}

frame* frame::copy() const
{
    frame *f = new frame;
    f->mySender = mySender;
    f->myReceiver = myReceiver;
    f->myType = myType;
    f->myDifferent = myDifferent;
    f->data = data;
    return f;
}




