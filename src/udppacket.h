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
    void setSender( quint16 i ) { mySender = i;}
    void setReceiver( quint16 i ) { myReceiver = i; }
    quint16 sender() const { return mySender; }
    quint16 receiver() const { return myReceiver; }
private:
    quint16 mySender; //!< Порт отправителя
    quint16 myReceiver; //!< Порт получателя
    QByteArray data; //!< Поле данных
protected:
    friend QDataStream& operator<<( QDataStream &stream, const udpPacket &p );
    friend QDataStream& operator>>( QDataStream &stream, udpPacket &p );
};

#endif // UDPPACKET_H
