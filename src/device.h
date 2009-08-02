#ifndef DEVICE_H
#define DEVICE_H

#include <QGraphicsItem>
#include <QDataStream>
#include <QPainter>
#include "deviceimpl.h"

class cableDev;

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
    QRect pixmapRect;
    device(int t);
    device(QDataStream &stream);
    ~device();
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget);
    QRectF boundingRect() const {
        return devRect;
    }
    bool isBusy() const { return impl->isBusy(); }
    void updateCables();
    void setMenu(QMenu *context) { popUpMenu = context; }
    QString tableName() { return impl->tableName(); }
    QString socketName(const cableDev *c) const { return impl->socketName(c); }
    bool isSmart() const { return impl->isSmart(); }
    bool isConnect() const { return myCableList.count(); }
    void dialog() { impl->dialog(); setToolTip( impl->note() ); }
    void tableDialog() const { impl->tableDialog(); }
    void adapterDialog() const { impl->adapterDialog(); }
    void programmsDialog() const { impl->programmsDialog(); }
    bool isCanSend() const { return impl->isCanSend() && isConnect() ; }
    bool isConnectSocket(const QString &str) { return impl->isConnectSocket(str); }
    QStringList sockets() const { return impl->sockets(); }
    void secondTimerEvent() { impl->secondTimerEvent(); }
    void deciSecondTimerEvent() { impl->deciSecondTimerEvent(); }
    deviceImpl* contentDevice() { return impl; }
    void addConnection(const QString &port, cableDev *c);
    void deleteConnection(cableDev *c);
    void sendMessage(const QString &a ,int size , int pr) { impl->sendMessage(a,size,pr); }

    static bool isConnectDevices(device *s, device *e);
    QList<cableDev*> cables() const { return myCableList; }
    bool hasTable() const { return impl->hasTable(); }
private:
    deviceImpl *impl;
    QMenu *popUpMenu; //!< Всплывающее меню для устройства
protected:
    QList<cableDev*> myCableList; //!< Список всех подключеных проводов.
    friend QDataStream& operator<<(QDataStream &stream,const device &dev);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event); // Событие контекстного меню
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
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

#endif // DEVICE_H
