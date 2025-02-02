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

class Cable;
class logDialog;
class DevicePort;

/*!
  Устройство, это основная единица с которой мы имеем дело в программе,
*/
class Device : public QGraphicsObject , public Visualizable
{
    Q_OBJECT
public:
    enum sizeDevices { rectDevX = -23 , rectDevY = -23 , rectDevWidth = 46 , rectDevHeight = 46 };
    enum { noDev = 0 , busDev = 2 ,compDev = 3 , hubDev = 4 , switchDev = 5 , routerDev = 7 };
    enum { Type = UserType + 2 };
    int type() const { return Type; }
    int deviceType() const { return impl->type(); }
    QRect devRect;
    QRect pixmapRect;
    Device( QObject* = 0);
    Device(int t);
    Device(QDataStream &stream);    
    ~Device();
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget);
    QRectF boundingRect() const {
        return devRect;
    }
    bool isBusy() const { return impl->isBusy(); }
    void updateCables();
    void setMenu(QMenu *context) { popUpMenu = context; }
    QString tableName() { return impl->tableName(); }
    QString socketName(const Cable *c) const { return impl->socketName(c); }
    QString deviceName() const { return impl->deviceName(); }
    QString deviceCommandName() const { return impl->deviceCommandName(); }
    bool isSmart() const { return impl->isSmart(); }
    bool isConnect() const { return impl->isConnect(); }
    bool isManagedVirtualNetwork() const { return impl->isManagedVirtualNetwork(); }
    void dialog() { impl->dialog(); }
    void tableDialog() const { impl->tableDialog(); }
    void adapterDialog() const { impl->adapterDialog(); }
    void programmsDialog() const { impl->programsDialog(); }
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
    DeviceImpl* contentDevice() { return impl; }
    DevicePort* findPortByName(const QString &name) const { return impl->findPortByName(name); }

    void sendMessage(const QString &a ,int size , int pr) { impl->sendMessage(a,size,pr); }
    Statistics deviceStatistics() const { return impl->deviceStatistics(); }

    Q_INVOKABLE void setDeviceImpl(DeviceImpl *im);

    void detectCollision() { impl->detectCollision(); }
    static bool isConnectDevices(Device *s, Device *e);
    QList<Cable*> cables() const { return impl->cableList(); }
    bool hasTable() const { return impl->hasTable(); }    

    void onImplChange();
private:
    void createImpl(int n);
    void createImplHelper();
    DeviceImpl *impl;
    QMenu *popUpMenu; //!< Всплывающее меню для устройства
    FeaturesMap myFeatures;
protected:    
    friend QDataStream& operator<<(QDataStream &stream,const Device &dev);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event); // Событие контекстного меню
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};
//--------------------------------------------------------------------
/*!
  Записывает устройство в поток.
*/
inline QDataStream& operator<<(QDataStream &stream,const Device &dev)
{
    stream << dev.pos();
    dev.impl->write(stream);
    return stream;
}
//--------------------------------------------------------------------

#endif // DEVICE_H
