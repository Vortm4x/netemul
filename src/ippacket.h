#ifndef IPPACKET_H
#define IPPACKET_H

#include <QSharedData>
#include "ipaddress.h"

/*!
  * Содержит в себе разделяемые между ip пакетами данные.
*/
class ipPacketData : public QSharedData
{
public:
    ipPacketData() { }
    ipPacketData(const ipPacketData &other);
    ~ipPacketData() {  }
    friend class ipPacket;
private:
    ipAddress sender; //!< Адрес отправителя.
    ipAddress receiver; //!< Адрес получателя.
    qint8 upProtocol; //!< Протокол верхнего уровня
    QByteArray data; //!< Данные протокола более высокого уровня.
    QByteArray toData() const;
};


/*!
  Реализует ip-пакет, так же как и в реальной сети содержит адрес отправителя, получателя,
  тип протокола верхнего уровня и поле данных. Остальные папраметры реального пакета пока
  не используются.
*/
class ipPacket
{
public:
    /*! Используется для обозначения протокола верхнего уровня. */
    enum { udp = 0 , tcp = 1 };
    ipPacket() { d = new ipPacketData; }
    ipPacket(const QByteArray &b);
    ipPacket(ipAddress s,ipAddress r);
    ~ipPacket() { }
    ipPacket(const ipPacket &other) : d(other.d) { }
    QByteArray toData() const;
    ipAddress sender() const { return d->sender; }
    ipAddress receiver() const { return d->receiver; }
    void setSender(ipAddress a) { d->sender = a; }
    void setReceiver(ipAddress a) { d->receiver = a; }
    bool isBroadcast(const ipAddress mask) const;
    void setBroadcast(const ipAddress mask);
    ipPacket& operator=(const ipPacket &other);
    void setUpProtocol(qint8 u) { d->upProtocol = u; }
    qint8 upProtocol() const { return d->upProtocol; }
    void pack(const QByteArray &b) { d->data = b; }
    int size() { return d->data.size(); }
    QByteArray unpack() const  { return d->data; }
private:
    QSharedDataPointer<ipPacketData> d; //!< Данные пакета.
protected:
    friend bool operator==(const ipPacket &p1,const ipPacket &p2);
    friend QDataStream& operator<<(QDataStream &stream,const ipPacket &p);
};
//---------------------------------------------------------------------------
/*!
  Записывает ip-пакет в поток.
  @param stream - поток для записи.
  @param p - записываемый пакет.
  @return ссылку на результирующий поток.
*/
inline QDataStream& operator<<(QDataStream &stream,const ipPacket &p)
{
    stream << p.toData();
    return stream;
}
//--------------------------------------------------
/*!
  Назначает пакету широковещательный адрес исходя из маски.
  @param mask - Маска.
*/
inline void ipPacket::setBroadcast(const ipAddress mask)
{
    d->receiver = d->sender | ~mask;
}
//---------------------------------------------------
inline bool operator==(const ipPacket &p1,const ipPacket &p2)
{
    return ( p1.sender() == p2.sender() && p1.receiver() == p2.receiver() );
}

#endif // IPPACKET_H
