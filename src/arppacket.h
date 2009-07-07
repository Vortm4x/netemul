#ifndef ARPPACKET_H
#define ARPPACKET_H

#include "macaddress.h"
#include "ipaddress.h"

class arpPacket
{
public:
    arpPacket(macAddress rm , macAddress sm , ipAddress ri , ipAddress si , quint8 t);
    arpPacket(const arpPacket &other);
    arpPacket() { }
    ~arpPacket() { }
    enum { request = 0 , answer = 1 };
    void setType(int i) { myType = i; }
    int type() const { return myType; }
    void setSenderMac(macAddress m) { mySenderMac = m; }
    void setSenderIp(ipAddress a) { mySenderIp = a; }
    void setReceiverMac(macAddress m) { myReceiverMac = m; }
    void setReceiverIp(ipAddress a) { myReceiverIp = a; }
    macAddress senderMac() const { return mySenderMac; }
    ipAddress senderIp() const { return mySenderIp; }
    macAddress receiverMac() const  { return myReceiverMac; }
    ipAddress receiverIp() const { return myReceiverIp; }
    arpPacket& operator=(arpPacket &other);
private:
    quint8 myType;
    macAddress mySenderMac;
    macAddress myReceiverMac;
    ipAddress mySenderIp;
    ipAddress myReceiverIp;
protected:
    friend QDataStream& operator<<(QDataStream &stream,const arpPacket &p);
    friend QDataStream& operator>>(QDataStream &stream,arpPacket &p);
};
//-------------------------------------------------------------------
/*!
  Записывает arp-пакет в поток.
  @param stream - поток для записи.
  @param p - пакет для записи.
  @return обновленный поток.
*/
inline QDataStream& operator<<(QDataStream &stream,const arpPacket &p)
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
inline QDataStream& operator>>(QDataStream &stream,arpPacket &p)
{
    stream >> p.myType;
    stream >> p.mySenderMac;
    stream >> p.myReceiverMac;
    stream >> p.mySenderIp;
    stream >> p.myReceiverIp;
    return stream;
}
//--------------------------------------------------------------

#endif // ARPPACKET_H
