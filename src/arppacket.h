#ifndef ARPPACKET_H
#define ARPPACKET_H

#include <QSharedData>
#include "macaddress.h"
#include "ipaddress.h"


class arpPacketData : public QSharedData
{
public:
    arpPacketData() { }
    arpPacketData(const arpPacketData &other);
    ~arpPacketData() { }
    QByteArray toData() const;
    friend class arpPacket;
private:
    quint8 type;
    macAddress senderMac;
    macAddress receiverMac;
    ipAddress senderIp;
    ipAddress receiverIp;
};

class arpPacket
{
public:
    arpPacket(macAddress rm , macAddress sm , ipAddress ri , ipAddress si , quint8 t);
    arpPacket(const arpPacket &other) : d(other.d) { }
    arpPacket() { d = new arpPacketData; }
    arpPacket(const QByteArray &b);
    QByteArray toData() const;
    enum { request = 0 , response = 1 };
    void setType(int i) { d->type = i; }
    int type() const { return d->type; }
    void setSenderMac(macAddress m) { d->senderMac = m; }
    void setSenderIp(ipAddress a) { d->senderIp = a; }
    void setReceiverMac(macAddress m) { d->receiverMac = m; }
    void setReceiverIp(ipAddress a) { d->receiverIp = a; }
    macAddress senderMac() const { return d->senderMac; }
    ipAddress senderIp() const { return d->senderIp; }
    macAddress receiverMac() const  { return d->receiverMac; }
    ipAddress receiverIp() const { return d->receiverIp; }
private:
    QSharedDataPointer<arpPacketData> d;
protected:
    friend QDataStream& operator<<(QDataStream &stream,const arpPacket &p);
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
    stream << p.toData();
    return stream;
}
//------------------------------------------------------------

#endif // ARPPACKET_H
