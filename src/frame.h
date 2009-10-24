/****************************************************************************************
** NetEmul - program for simulating computer networks.
** Copyright © 2009 Semenov Pavel and Omilaeva Anastasia
**
** NetEmul is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** NetEmul is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with the NetEmul; if not, write to the Free
** Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
** 02111-1307 USA.
****************************************************************************************/
#ifndef FRAME_H
#define FRAME_H

#include "macaddress.h"
#include <QSharedData>

/*!
  * Класс данные общего пользования, для фреймов.
*/
class frameData : public QSharedData {
public:
    frameData();
    frameData(const frameData &other) : QSharedData(other) , sender(other.sender) ,
                                    receiver(other.receiver) , type(other.type) ,
                                    different(other.different) , data(other.data) { }
    ~frameData() {  }
    QByteArray toArray() const;
    friend class frame;
private:
    macAddress sender; //!< Mac-адрес отправителя
    macAddress receiver; //!< Mac-адрес получателя
    qint8 type; //!< Показывает несет ли в себе кадр ip-пакет или arp сообщение.
    qint8 different; //!< Разновидность фрэйма, влияет на его цвет и отображение
    QByteArray data; //!< Данные протокола более высокого уровня.
};

/*!
  Реализует кадр, также как и в реальной сети содержит адрес отправителя, получателя и
  поле данных где содержиться информация протокола более высокого уровня.
*/
class frame
{
public:
    enum { arp = 100 , ip = 101 };
    enum { NORMAL = 3 , BROADCAST = 4 };
    frame() { d = new frameData; }
    frame(const frame &other) : d(other.d) { }
    frame(const QByteArray &b);
    macAddress sender() const { return d->sender; }
    void setSender(macAddress temp) { d->sender = temp; }
    macAddress receiver() const { return d->receiver; }
    void setReceiver(macAddress temp) { d->receiver = temp;  }
    void setDifferent(qint8 t) { d->different = t; }
    int type() const { return d->type; }
    void setType(int t) { d->type = t; }
    QByteArray toData() const;
    QString toString() const;
    void pack(const QByteArray &b) { d->data = b; }
    QByteArray unpack() const { return d->data; }
private:
    QSharedDataPointer<frameData> d;
protected:
    friend QDataStream& operator<<(QDataStream &stream, const frame &f);
};

/*!
  Записывает кадр в поток.
  @param stream - ссылка на поток.
  @param f - ссылка на кадр.
  @return результирующий поток.
*/
inline QDataStream& operator<<(QDataStream &stream, const frame &f)
{
    stream << f.toData();
    return stream;
}
//----------------------------------------------------


#endif // FRAME_H
