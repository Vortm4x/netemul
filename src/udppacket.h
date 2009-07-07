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
    udpPacket(const udpPacket &u);
    ~udpPacket() { }
    udpPacket& operator=(const udpPacket &u);
    void setSender( quint16 i ) { mySender = i;}
    void setReceiver( quint16 i ) { myReceiver = i; }
    void setData(QByteArray d) { myData = d; }
    const QByteArray& data() const { return myData; }
    quint16 sender() const { return mySender; }
    quint16 receiver() const { return myReceiver; }
private:
    quint16 mySender; //!< Порт отправителя
    quint16 myReceiver; //!< Порт получателя
    QByteArray myData; //!< Поле данных
protected:
    friend QDataStream& operator<<( QDataStream &stream, const udpPacket &p );
    friend QDataStream& operator>>( QDataStream &stream, udpPacket &p );
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
    stream << p.myData;
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
    stream >> p.myData;
    return stream;
}
//-------------------------------------------------------
#endif // UDPPACKET_H
