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
#ifndef DEVICE_H
#define DEVICE_H

#include <QGraphicsItem>
#include <QDataStream>
#include <QIcon>
#include "deviceimpl.h"
#include "statistics.h"
#include "visualizable.h"

class cableDev;
class logDialog;

/*!
  Устройство, это основная единица с которой мы имеем дело в программе,
  хотя и являеться абстрактным классом оно уже подерживает не малую функциональность.
  Класс содержит несколько виртуальных функций, только переопределив которые, мы сможем его унаследовать.
*/
class device : public QGraphicsItem , public visualizable
{
public:
    enum sizeDevices { rectDevX = -23 , rectDevY = -23 , rectDevWidth = 46 , rectDevHeight = 46 };
    enum { noDev = 0 , busDev = 2 ,compDev = 3 , hubDev = 4 , switchDev = 5 , routerDev = 7 };
    enum { Type = UserType + 2 };
    int type() const { return Type; }
    int deviceType() const { return impl->type(); }
    QRect devRect;
    QRect pixmapRect;
    device(int t);
    device(QDataStream &stream);
    device(sceneXmlReader &stream);
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
    QString deviceName() const { return impl->deviceName(); }
    QString deviceCommandName() const { return impl->deviceCommandName(); }
    bool isSmart() const { return impl->isSmart(); }
    bool isConnect() const { return myCableList.count(); }
    bool isManagedVirtualNetwork() const { return impl->isManagedVirtualNetwork(); }
    void dialog() { impl->dialog(); }
    void tableDialog() const { impl->tableDialog(); }
    void adapterDialog() const { impl->adapterDialog(); }
    void programmsDialog() const { impl->programmsDialog(); }
    void arpDialog() const { impl->arpDialog(); }
    void showLogDialog(logDialog *log) const { impl->showLogDialog(log); }
    void showDesignerDialog() const { impl->showDesignerDialog(); }
    void showDeviceNoteDialog() { impl->showDeviceNoteDialog(); setToolTip( impl->note() ); }
    void showVirtualNetworkDialog() const {  impl->showVirtualNetworkDialog(); }
    bool isCanSend() const { return impl->isCanSend() && isConnect() ; }
    bool isShared() const { return impl->isShared(); }
    bool isConnectSocket(const QString &str) const { return impl->isConnectSocket(str); }
    QIcon isConnectSocketIcon(const QString &str) const { return impl->isConnectSocketIcon(str); }
    QStringList sockets() const { return impl->sockets(); }
    void secondTimerEvent() { impl->secondTimerEvent(); }
    void deciSecondTimerEvent() { impl->deciSecondTimerEvent(); }
    deviceImpl* contentDevice() { return impl; }
    void addConnection(const QString &port, cableDev *c);
    void deleteConnection(cableDev *c);
    void sendMessage(const QString &a ,int size , int pr) { impl->sendMessage(a,size,pr); }
    statistics deviceStatistics() const { return impl->deviceStatistics(); }

    void detectCollision() { impl->detectCollision(); }
    static bool isConnectDevices(device *s, device *e);
    QList<cableDev*> cables() const { return myCableList; }
    bool hasTable() const { return impl->hasTable(); }

    void writeXml(sceneXmlWriter &stream) const;

    void onImplChange();
private:
    void createImpl(int n);
    deviceImpl *impl;
    QMenu *popUpMenu; //!< Всплывающее меню для устройства
    featuresMap myFeatures;
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
