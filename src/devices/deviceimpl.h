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
#ifndef DEVICEIMPL_H
#define DEVICEIMPL_H


#include <QtScript>
#include "scenexmlwriter.h"
#include "scenexmlreader.h"
#include "statistics.h"
#include "visualizable.h"

class Cable;
class logDialog;
class DevicePort;

typedef QMap<QString,bool> featuresMap;

class DeviceImpl : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString note READ note WRITE setNote)
    Q_PROPERTY(bool router READ isRouter WRITE setRouter)
public:
    DeviceImpl(QObject *parent = 0);
    void showDeviceNoteDialog();
    virtual ~DeviceImpl() { }

    virtual int type() const = 0;

    virtual QStringList sockets() const = 0;
    virtual featuresMap featuresList() const { return featuresMap(); }
    virtual bool isConnectSocket(const QString &socket) const = 0;
    virtual QIcon isConnectSocketIcon( const QString &socket) const = 0;
    virtual QString socketName(const Cable *c) const = 0;
    virtual QString pixmapName() const = 0;

    virtual bool isBusy() const = 0;
    virtual bool isConnect() const = 0;

    const QString& note() const { return myNote; }
    void setNote(const QString &n) { myNote = n; }

    virtual QString tableName() const { return QString(); }
    virtual bool isSmart() const { return false; }

    virtual bool hasTable() const { return false; }
    virtual void dialog() = 0;
    virtual void showLogDialog(logDialog*) const = 0;
    virtual void tableDialog() { qFatal("error not compatibility Table!"); }
    virtual void adapterDialog() { qFatal("error not compatibility adapter!"); }
    virtual void programmsDialog() { qFatal("error not compatibility"); }
    virtual void arpDialog() { qFatal("error not compatibility"); }
    virtual void showDesignerDialog() { qFatal("Error designer!"); }
    virtual void showVirtualNetworkDialog() { qFatal("Error virtual network"); }

    virtual bool isShared() const { return false; }
    virtual bool isManagedVirtualNetwork() const { return false; }
// Writing and reading
    virtual void write(QDataStream &stream) const;
    virtual void read(QDataStream &stream);

    virtual QString deviceName() const = 0;
    virtual QString deviceCommandName() const = 0;
    virtual void secondTimerEvent() { }
    virtual void deciSecondTimerEvent() { }
    virtual QString nameToIp(const QString&) const { return QString(); }

    virtual bool isReady() const { return true; }

    virtual QList<Cable*> cableList() const = 0;
    virtual DevicePort* findPortByName(const QString&) const = 0;

    virtual bool isCanSend() const { return false; }

    void setVisualizator( visualizable *view ) { myView = view; }
    void updateView() const { myView->onImplChange(); }
public:    

    virtual Statistics deviceStatistics() const = 0;
    virtual int trafficDigit() const = 0;

    virtual bool isRouter() const { return false; }
    virtual void setRouter(bool) { }
    virtual void detectCollision() { }

public slots:
    virtual void setCheckedSocket(const QString&) { }
    virtual void setIp(const QString&, const QString&) { }
    virtual void setMask(const QString&, const QString&) { }
    virtual void setGateway(const QString&) { }
    virtual void setSocketsCount(int) { }
    virtual void sendMessage(const QString& ,int, int) { qFatal("ERROR!"); }
    virtual quint64 sendPacketCount(const QString&) { return 0; }
    virtual quint64 receivePacketCount(const QString&) { return 0; }
    virtual quint64 sendFrameCount(const QString&) { return 0; }
    virtual quint64 receiveFrameCount(const QString&) { return 0; }
protected:
    visualizable *myView;
private:
    QString myNote;
};

class deviceSetting {
public:
    deviceSetting(DeviceImpl *d) : dev(d) { }
    QString note() const { return dev->note(); }
    void setNote(const QString &str) { dev->setNote(str); }
private:
    DeviceImpl *dev;
};

Q_DECLARE_METATYPE(DeviceImpl*)

#endif // DEVICEIMPL_H
