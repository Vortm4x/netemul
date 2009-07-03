#ifndef TCPPACKET_H
#define TCPPACKET_H

#include <QDataStream>

/*!
  Реализует tcp-сегмент
*/
class tcpPacket
{
public:
    tcpPacket();
    void setSender( quint16 i ) { mySender = i;}
    void setReceiver( quint16 i ) { myReceiver = i; }
    quint16 sender() const { return mySender; }
    quint16 receiver() const { return myReceiver; }
private:
    quint16 mySender; //!< Порт отправителя
    quint16 myReceiver; //!< Порт получателя
    QByteArray data; //!< Поле данных
protected:
    friend QDataStream& operator<<( QDataStream &stream, const tcpPacket &p );
    friend QDataStream& operator>>( QDataStream &stream, tcpPacket &p );
};

#endif // TCPPACKET_H
