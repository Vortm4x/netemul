#ifndef FRAME_H
#define FRAME_H

#include "macaddress.h"
#include <QBrush>
#include "ippacket.h"
#include "arppacket.h"
/*!
  Реализует кадр, также как и в реальной сети содержит адрес отправителя, получателя и
  поле данных где содержиться информация протокола более высокого уровня.
*/
class frame
{
public:
    enum { startToEnd = 0 , endToStart = 1 , arp = 100 , ip = 101 };
    void setPos(qreal temp) { myPos = temp; }
    qreal pos() { return myPos; }
    /*! Возвращает направление движения пакета. */
    qint8 direct() const { return myDirect; }
    /*! Задает напрваление движения пакета. */
    void setDirection(qint8 t) { myDirect = t; }
    frame();
    ~frame();
    frame operator=(frame &other);
    macAddress sender() const { return mySender; }
    void setSender(macAddress temp) { mySender = temp; }
    macAddress receiver() const { return myReceiver; }
    void setReceiver(macAddress temp) { myReceiver = temp; if ( temp.isBroadcast() ) setColor(Qt::yellow);
                                                           else setColor( Qt::red );  }
    QBrush& color() { return myColor; }
    void setColor(QBrush b) { myColor = b; }
    int type() const { return myType; }
    void setType(int t) { myType = t; }
    void operator<<(arpPacket &p);
    void operator>>(arpPacket &p) const;
    void operator<<(ipPacket &p);
    void operator>>(ipPacket &p) const;
private:
    macAddress mySender;
    macAddress myReceiver;
    QBrush myColor;
    qreal myPos;
    qint8 myDirect;
    qint8 myType; //!< Показывает несет ли в себе кадр ip-пакет или arp сообщение.
    QByteArray data; //!< Данные протокола более высокого уровня.
};

/*!
    Упаковывает arp-сообщение в кадр.
    @param p - arp-сообщение.
*/
inline void frame::operator<<(arpPacket &p)
{
    QDataStream in(&data,QIODevice::WriteOnly);
    in << p;
}
//--------------------------------------------
/*!
    Извлекает arp-сообщение из кадра.
    @param p - arp-сообщение в которое извлекаем.
*/
inline void frame::operator>>(arpPacket &p) const
{
    QDataStream out(data);
    out >> p;
}
//--------------------------------------------
/*!
    Упаковывает ip-пакет в кадр.
    @param p - ip-пакет.
*/
inline void frame::operator<<(ipPacket &p)
{
    QDataStream in(&data,QIODevice::WriteOnly);
    in << p;
}
//-----------------------------------------------
/*!
    Извлекает ip-пакет из кадра.
    @param p - ip-пакет в который извлекаем.
*/
inline void frame::operator>>(ipPacket &p) const
{
    QDataStream out(data);
    out >> p;
}
//------------------------------------------------



#endif // FRAME_H
