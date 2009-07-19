#ifndef DEVICE_H
#define DEVICE_H

#include <QGraphicsItem>
#include <QDataStream>
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
    enum { Type = UserType + 2 };
    int type() const { return Type; }
    QRect devRect;
    device(int t);
    device(QDataStream &stream);
    ~device();
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget);
    QRectF boundingRect() const {
        return devRect;
    }
    void setMenu(QMenu *context) { popUpMenu = context; }
    QString tableName() { return impl->tableName(); }
    bool isSmart() const { return impl->isSmart(); }
    bool isConnect() { return myCableList.count(); }
    void dialog() { impl->dialog(); }
    void tableDialog() const { impl->tableDialog(); }
    void adapterDialog() const { impl->adapterDialog(); }
    void programmsDialog() const { impl->programmsDialog(); }
    bool isCanSend() const { return impl->isCanSend(); }
    void sendMessage(ipAddress dest, int size , int pr) { impl->sendMessage(dest,size,pr); }
    devicePort* socket(const QString &str) { return impl->socket(str); }
    QString socketName(devicePort *p) { return impl->socketName(p); }
    bool isConnectSocket(const QString &str) { return impl->isConnectSocket(str); }
    QStringList sockets() const { return impl->sockets(); }
    void secondTimerEvent() { impl->secondTimerEvent(); }
    void deciSecondTimerEvent() { impl->deciSecondTimerEvent(); }

    virtual void addConnection(cableDev *cable) { myCableList << cable;}
    virtual void deleteConnection(cableDev *cable) { myCableList.removeOne(cable); }
    QList<cableDev*> cables() const { return myCableList; }
    QString nextName() { return QString("eth%1").arg(count++); }
    void sendEvent();
    virtual void showTable() { }
    bool hasTable() const { return impl->hasTable(); }
private:
    deviceImpl *impl;
    QMenu *popUpMenu; //!< Всплывающее меню для устройства
protected:
    int myReady;
    QList<cableDev*> myCableList; //!< Список всех подключеных проводов.
    friend QDataStream& operator<<(QDataStream &stream,const device &dev);
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
    stream << dev.pos();
    dev.impl->write(stream);
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
