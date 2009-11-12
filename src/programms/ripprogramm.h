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
#ifndef RIPPROGRAMM_H
#define RIPPROGRAMM_H

#include "programmrep.h"

class smartDevice;
class routeModel;
class routeRecord;
class udpSocket;

static const qint8 RIP_INFINITY = 16;

/*!
  Реализует Rip-программу.
*/
class ripProgramm : public programmRep
{
    Q_OBJECT
    Q_PROPERTY( bool triggerUpdate READ hasTriggerUpdate WRITE setTriggerUpdate )
    Q_PROPERTY( int splitMode READ splitMode WRITE setSplitMode )
public:
    enum { defaultTtl = 30 , RIP = 0 , ttl = 6 };
    enum { SPLIT_NONE , SPLIT_HORIZONT , SPLIT_WIH_POISON };
    int id() const { return RIP; }
    ripProgramm();
    ~ripProgramm();
    void setDevice(smartDevice *s);
    bool interrupt(int u);
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
    void incTime();
    int splitMode() const { return mySplitMode; }
    void setSplitMode(int mode) { mySplitMode = mode; }
    bool hasTriggerUpdate() const { return myTriggerUpdate; }
    void setTriggerUpdate(bool has) { myTriggerUpdate = has; }
    void showProperty();
private slots:
    void execute(QByteArray data);
private:
    void checkTable( routeRecord *r );
    void sendUpdate(bool isAll);
    int mySplitMode;
    bool myTriggerUpdate;
    quint16 mySocket;
    udpSocket *receiver;
    void addToTemp(routeRecord *r);
    void clearTemp();
    QList<routeRecord*> tempList;
    int timer;
    int interval;
    routeModel *model;
};
//----------------------------------------------

#endif // RIPPROGRAMM_H
