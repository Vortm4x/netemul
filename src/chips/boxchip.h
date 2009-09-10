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

class cableDev;
class devicePort;

class boxChip : public abstractChip
{
    Q_OBJECT
public:
    boxChip(int n = 4);
    virtual ~boxChip();
    QStringList sockets() const;
    bool setSocketsCount(int n);
    int socketsCount() const { return mySockets.size(); }
    bool isConnectSocket(const QString &str) const;
    void addConnection(const QString &port, cableDev *c);
    void deleteConnection(cableDev *c);
    int trafficDigit() const;
    QString socketName(const cableDev *c) const;
    virtual void deciSecondTimerEvent();
    virtual void secondTimerEvent() { }
    devicePort* socket(const QString &name);
    bool isBusy() const;
    virtual void addSocket(int n);
    virtual bool isSharedBusy(cableDev*) const { return false; }
    virtual void detectCollision() { }
    void setCheckedSocket(const QString &port);
#ifndef __TESTING__
    virtual void write(QDataStream &stream) const;
    virtual void read(QDataStream &stream);
#endif  
protected:
    QVector<devicePort*> mySockets;
};

#endif // BOXCHIP_H
