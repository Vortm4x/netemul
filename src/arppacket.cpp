#include "arppacket.h"

arpPacket::arpPacket(macAddress rm , macAddress sm , ipAddress ri , ipAddress si , int t)
{
    myReceiverMac = rm;
    mySenderMac = sm;
    myReceiverIp = ri;
    mySenderIp = si;
    myType = t;
}

arpPacket arpPacket::operator=(arpPacket other)
{
    mySenderMac = other.mySenderMac;
    myReceiverMac = other.myReceiverMac;
    mySenderIp = other.mySenderIp;
    myReceiverIp = other.myReceiverIp;
    myType = other.myType;
    return *this;
}
/*!
  Записывает arp-пакет в поток.
  @param stream - поток для записи.
  @param p - пакет для записи.
  @return обновленный поток.
*/
QDataStream& operator<<(QDataStream &stream,const arpPacket &p)
{
    stream << p.myType;
    stream << p.mySenderMac;
    stream << p.myReceiverMac;
    stream << p.mySenderIp;
    stream << p.myReceiverIp;
    return stream;
}
//------------------------------------------------------------
/*!
  Извлекает arp-пакет из потока.
  @param stream - поток для извлечения.
  @param p - пакет в который извлекаем.
  @return обновленный поток.
*/
QDataStream& operator>>(QDataStream &stream,arpPacket &p)
{
    stream >> p.myType;
    stream >> p.mySenderMac;
    stream >> p.myReceiverMac;
    stream >> p.mySenderIp;
    stream >> p.myReceiverIp;
    return stream;
}
//--------------------------------------------------------------
