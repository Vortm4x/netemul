#ifndef FRAME_H
#define FRAME_H

#include "macaddress.h"

/*!
  Реализует кадр, также как и в реальной сети содержит адрес отправителя, получателя и
  поле данных где содержиться информация протокола более высокого уровня.
*/
class frame
{
public:
    enum { arp = 100 , ip = 101 };
    enum { normal = 3 , broadcast = 4 };
    frame();
    frame(const frame &other);
    frame(const QByteArray &b);
    ~frame();
    frame* copy() const;
    frame& operator=(const frame &other);
    macAddress sender() const { return mySender; }
    void setSender(macAddress temp) { mySender = temp; }
    macAddress receiver() const { return myReceiver; }
    void setReceiver(macAddress temp) { myReceiver = temp;  }
    void setDifferent(qint8 d) { myDifferent = d; }
    int type() const { return myType; }
    void setType(int t) { myType = t; }
    QByteArray toData() const;
    void pack(const QByteArray &b) { data = b; }
    QByteArray& unpack() { return data; }
private:
    macAddress mySender; //!< Mac-адрес отправителя
    macAddress myReceiver; //!< Mac-адрес получателя
    qint8 myType; //!< Показывает несет ли в себе кадр ip-пакет или arp сообщение.
    qint8 myDifferent; //!< Разновидность фрэйма, влияет на его цвет и отображение
    QByteArray data; //!< Данные протокола более высокого уровня.
protected:
    friend QDataStream& operator<<(QDataStream &stream, const frame &f);
    friend QDataStream& operator>>(QDataStream &stream, frame &f);
};

/*!
  Записывает кадр в поток.
  @param stream - ссылка на поток.
  @param f - ссылка на кадр.
  @return результирующий поток.
*/
inline QDataStream& operator<<(QDataStream &stream, const frame &f)
{
    stream << f.myDifferent << f.mySender << f.myReceiver << f.myType << f.data;
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
    stream >> f.myDifferent >> f.mySender >> f.myReceiver >> f.myType  >> f.data;
    return stream;
}
//----------------------------------------------------


#endif // FRAME_H
