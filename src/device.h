#ifndef DEVICE_H
#define DEVICE_H

#include <QGraphicsItem>
#include <QDataStream>
#include <QtScript>
#include <QPainter>
#include "deviceimpl.h"

class cableDev;
class QMenu;

/*!
  Устройство, это основная единица с которой мы имеем дело в программе,
  хотя и являеться абстрактным классом оно уже подерживает не малую функциональность.
  Класс содержит несколько виртуальных функций, только переопределив которые, мы сможем его унаследовать.
*/
class device : public QGraphicsItem
{
public:
    enum sizeDevices { rectDevX = -23 , rectDevY = -23 , rectDevWidth = 46 , rectDevHeight = 46 };
    enum { noDev = 0 , busDev = 2 ,compDev = 3 , hubDev = 4 , switchDev = 5 , routerDev = 7 };
    QRect devRect;
    device();
    ~device();
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget);
    QList<devicePort*> sockets() { return impl->sockets(); }
    devicePort* socket(const QString &s) { return impl->socket(s); }
    QRectF boundingRect() const {
        return devRect;
    }
    void setMenu(QMenu *context) { popUpMenu = context; }
    bool isConnect() { return myCableList.count(); }
    virtual void addConnection(cableDev *cable) { myCableList << cable;}
    virtual void deleteConnection(cableDev *cable) { myCableList.removeOne(cable); }
    QList<cableDev*> cables() const { return myCableList; }
    QString nextName() { return QString("eth%1").arg(count++); }
    void sendEvent();
    void setId(int i) { myId = i; }
    int id() const { return myId; }
    virtual void showTable() { }
    virtual QPointF getPointCable(QPointF otherDev) = 0;
    virtual void write(QDataStream &stream) const = 0;
    virtual void read(QDataStream &stream) = 0 ;
    virtual void dialog() = 0;
    bool hasTable() const { return impl->hasTable(); }
    bool accupant();
private:
    deviceImpl *impl;
    QMenu *popUpMenu; //!< Всплывающее меню для устройства
    int myId; //!< Подобие указателя для QtScript
protected:
    int myReady;
    QList<devicePort*> mySockets; //!< Список всех сокетов устройства.
    QList<cableDev*> myCableList; //!< Список всех подключеных проводов.
    friend QDataStream& operator<<(QDataStream &stream,const device &dev);
    friend QDataStream& operator>>(QDataStream &stream,device &dev);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event); // Событие контекстного меню
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    int count;
};
//--------------------------------------------------------------------
/*!
  Записывает устройство в поток.
*/
inline QDataStream& operator<<(QDataStream &stream,const device &dev)
{
    dev.write(stream);
    return stream;
}
//--------------------------------------------------------------------
/*!
  Считывает устройство из потока.
*/
inline QDataStream& operator>>(QDataStream &stream,device &dev)
{
    dev.read(stream);
    return stream;
}
//--------------------------------------------------------------------
/*!
  Посылает пакеты с устройства.
*/
inline void device::sendEvent()
{
    impl->sendEvent();
}
//--------------------------------------------------------------------
#endif // DEVICE_H
