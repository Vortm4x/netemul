#include "frame.h"
#include <QDataStream>

frame::frame()
{
    myColor = Qt::red;
}

frame::~frame()
{

}

frame frame::operator=(frame other)
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

/*!
    Упаковывает arp-сообщение в кадр.
    @param p - arp-сообщение.
*/
void frame::operator<<(arpPacket &p)
{
    QDataStream in(&data,QIODevice::WriteOnly);
    in << p;
}
//--------------------------------------------
/*!
    Извлекает arp-сообщение из кадра.
    @param p - arp-сообщение в которое извлекаем.
*/
void frame::operator>>(arpPacket &p) const
{
    QDataStream out(data);
    out >> p;
}
//--------------------------------------------
/*!
    Упаковывает ip-пакет в кадр.
    @param p - ip-пакет.
*/
void frame::operator<<(ipPacket &p)
{
    QDataStream in(&data,QIODevice::WriteOnly);
    in << p;
}
//-----------------------------------------------
/*!
    Извлекает ip-пакет из кадра.
    @param p - ip-пакет в который извлекаем.
*/
void frame::operator>>(ipPacket &p) const
{
    QDataStream out(data);
    out >> p;
}
//------------------------------------------------
