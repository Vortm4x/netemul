#ifndef UDPPACKET_H
#define UDPPACKET_H

#include <QDataStream>


/*!
  Реализует udp-дейтаграмму
*/
class udpPacket
{
public:
    udpPacket();
    udpPacket(const QByteArray &b);
    udpPacket(const udpPacket &u);
    ~udpPacket() { }
    udpPacket& operator=(const udpPacket &u);
    QByteArray toData() const;
    int size() const { return data.size(); }
    void setSender( quint16 i ) { mySender = i;}
    void setReceiver( quint16 i ) { myReceiver = i; }
    quint16 sender() const { return mySender; }
    quint16 receiver() const { return myReceiver; }
    void pack(const QByteArray &b) { data = b; }
    QByteArray& unpack() { return data; }
protected:
    friend QDataStream& operator<<( QDataStream &stream, const udpPacket &p );
    friend QDataStream& operator>>( QDataStream &stream, udpPacket &p );
    quint16 mySender; //!< Порт отправителя
    quint16 myReceiver; //!< Порт получателя
    QByteArray data; //!< Поле данных
};
//-----------------------------------------------------
/*!
  Записывает udp-дейтаграмму в поток.
  @param stream - поток для записи.
  @param p - записываемый пакет.
  @return ссылку на результирующий поток.
*/
inline QDataStream& operator<<( QDataStream &stream, const udpPacket &p )
{
    stream << p.myReceiver;
    stream << p.mySender;
    stream << p.data;
    return stream;
}
//-------------------------------------------------------
/*!
  Извлекает из потока udp-дейтаграмму
  @param stream - поток для чтения.
  @param p - извлекаемый пакет.
  @return ссылку на результирующий поток.
*/
inline QDataStream& operator>>( QDataStream &stream, udpPacket &p )
{
    stream >> p.myReceiver;
    stream >> p.mySender;
    stream >> p.data;
    return stream;
}
//-------------------------------------------------------
#endif // UDPPACKET_H
