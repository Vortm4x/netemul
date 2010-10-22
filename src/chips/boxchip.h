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
#ifndef BOXCHIP_H
#define BOXCHIP_H

#include "abstractchip.h"
#include <QVector>

class Cable;
class DevicePort;

class BoxChip : public AbstractChip
{
    Q_OBJECT
    Q_PROPERTY( int socketsCount READ socketsCount WRITE setSocketsCount )
public:
    BoxChip(QObject *parent = 0);
    virtual ~BoxChip();
    QStringList sockets() const;
    bool setSocketsCount(int n);
    int socketsCount() const { return mySockets.size(); }
    bool isConnectSocket(const QString &str) const;
    void addConnection(const QString &port, Cable *c);
    void deleteConnection(Cable *c);
    int trafficDigit() const;
    QString socketName(const Cable *c) const;
    virtual void deciSecondTimerEvent();
    virtual void secondTimerEvent() { }
    DevicePort* socket(const QString &name);
    bool isBusy() const;
    bool isConnect() const;
    QList<Cable*> cableList() const;
    DevicePort* findPortByName(const QString &name) const;
    virtual void addSocket(int n);
    virtual bool isSharedBusy(Cable*) const { return false; }
    virtual void detectCollision() { }
    void setCheckedSocket(const QString &port);
    virtual void write(QDataStream &stream) const;
    virtual void read(QDataStream &stream);    

    QString portToString(DevicePort *port) const;

signals:
    void socketsCountChanged();

public slots:
    void onCableConnected(Cable *cable);

protected:
    QVector<DevicePort*> mySockets;
};

#endif // BOXCHIP_H
