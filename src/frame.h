#ifndef FRAME_H
#define FRAME_H

#include "macaddress.h"
#include "ippacket.h"
#include "arppacket.h"

/*!
  Реализует кадр, также как и в реальной сети содержит адрес отправителя, получателя и
  поле данных где содержиться информация протокола более высокого уровня.
*/
class frame
{
public:
    enum { arp = 100 , ip = 101 };
    frame();
    frame(const frame &other);
    ~frame();
    frame operator=(frame &other);
    macAddress sender() const { return mySender; }
    void setSender(macAddress temp) { mySender = temp; }
    macAddress receiver() const { return myReceiver; }
    void setReceiver(macAddress temp) { myReceiver = temp;  }
    int type() const { return myType; }
    void setType(int t) { myType = t; }
    void operator<<(arpPacket &p);
    void operator>>(arpPacket &p) const;
    void operator<<(ipPacket &p);
    void operator>>(ipPacket &p) const;
private:
    macAddress mySender; //!< Mac-адрес отправителя
    macAddress myReceiver; //!< Mac-адрес получателя
    qint8 myType; //!< Показывает несет ли в себе кадр ip-пакет или arp сообщение.
    QByteArray data; //!< Данные протокола более высокого уровня.
protected:
    friend QDataStream& operator<<(QDataStream &stream, const frame &f);
    friend QDataStream& operator>>(QDataStream &stream, frame &f);
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
/*!
  Записывает кадр в поток.
  @param stream - ссылка на поток.
  @param f - ссылка на кадр.
  @return результирующий поток.
*/
inline QDataStream& operator<<(QDataStream &stream, const frame &f)
{
    stream << f.mySender << f.myReceiver << f.myType << f.data;
    return stream;
}
//----------------------------------------------------
/*!
  Читает кадр из поток.
  @param stream - ссылка на поток.
  @param f - ссылка на кадр.
  @return результирующий поток.
*/
inline QDataStream& operator>>(QDataStream &stream,frame &f)
{
    stream >> f.mySender >> f.myReceiver >> f.myType >> f.data;
    return stream;
}
//----------------------------------------------------


#endif // FRAME_H
