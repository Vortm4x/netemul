#ifndef IPPACKET_H
#define IPPACKET_H

#include "ipaddress.h"
#include "udppacket.h"
#include "tcppacket.h"
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
    ipPacket();
    ~ipPacket() { }
    ipPacket(const ipPacket &other);
    ipPacket(ipAddress s,ipAddress r) { mySender = s ; myReceiver = r; }
    ipAddress sender() const { return mySender; }
    ipAddress receiver() const { return myReceiver; }
    void setSender(ipAddress a) { mySender = a; }
    void setReceiver(ipAddress a) { myReceiver = a; }
    bool isBroadcast(const ipAddress mask) const;
    bool isBroadcast(const QString str) const { return isBroadcast(ipAddress(str)); }
    void setBroadcast(const ipAddress mask);
    void setBroadcast(const QString str) { setBroadcast(ipAddress(str)); }
    ipPacket& operator=(const ipPacket &other);
    void setUpProtocol(qint8 u) { myUpProtocol = u; }
    qint8 upProtocol() const { return myUpProtocol; }
    void operator<<(tcpPacket &p);
    void operator>>(tcpPacket &p) const;
    void operator<<(udpPacket &p);
    void operator>>(udpPacket &p) const;
private:
    ipAddress mySender; //!< Адрес отправителя.
    ipAddress myReceiver; //!< Адрес получателя.
    qint8 myUpProtocol; //!< Протокол верхнего уровня
    QByteArray data; //!< Данные протокола более высокого уровня.
protected:
    friend QDataStream& operator<<(QDataStream &stream,const ipPacket &p);
    friend QDataStream& operator>>(QDataStream &stream,ipPacket &p);
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
    stream << p.mySender;
    stream << p.myReceiver;
    stream << p.myUpProtocol;
    stream << p.data;
    return stream;
}
//--------------------------------------------------
/*!
  Извлекает ip-пакет из потока.
  @param stream - поток для чтения.
  @param p - извлекаемый пакет.
  @return ссылку на результирующий поток.
*/
inline QDataStream& operator>>(QDataStream &stream,ipPacket &p)
{
    stream >> p.mySender;
    stream >> p.myReceiver;
    stream >> p.myUpProtocol;
    stream >> p.data;
    return stream;
}
//-------------------------------------------------------------
/*!
  Упаковывает в ip-пакет tcp-сегмент
  @param p - tcp-сегмент.
*/
inline void ipPacket::operator<<( tcpPacket &p )
{
    QDataStream in( &data, QIODevice::WriteOnly );
    in << p;
}
//------------------------------------------------------------
/*!
  Извлекает из ip-пакета tcp-сегмент
  @param p - tcp-сегмент.
*/
inline void ipPacket::operator>>( tcpPacket &p ) const
{
    QDataStream out( data );
    out >> p;
}
//-------------------------------------------------------------
/*!
  Упаковывает в ip-пакет udp-дейтаграмму
  @param p - udp-дейтаграмма.
*/
inline void ipPacket::operator<<( udpPacket &p )
{
    QDataStream in( &data, QIODevice::WriteOnly );
    in << p;
}
//-------------------------------------------------------------
/*!
  Извлекает из ip-пакета udp-дейтаграмму
  @param p - udp-дейтаграмма.
*/
inline void ipPacket::operator>>( udpPacket &p ) const
{
    QDataStream out( data );
    out >> p;
}
//------------------------------------------------------------
/*!
  Назначает пакету широковещательный адрес исходя из маски.
  @param mask - Маска.
*/
inline void ipPacket::setBroadcast(const ipAddress mask)
{
    myReceiver = mySender | ~mask;
}
//---------------------------------------------------

#endif // IPPACKET_H
