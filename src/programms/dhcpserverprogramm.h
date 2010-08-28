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
#ifndef DHCPSERVERPROGRAMM_H
#define DHCPSERVERPROGRAMM_H

#include "programmrep.h"
#include "dhcpdemon.h"


class dhcpServerProgramm : public programmRep
{
    Q_OBJECT
//    Q_PROPERTY( QString interfaceName READ interfaceName WRITE setInterfaceName )
//    Q_PROPERTY( QString beginIp READ beginIp WRITE setBeginIp )
//    Q_PROPERTY( QString endIp READ endIp WRITE setEndIp )
//    Q_PROPERTY( QString mask READ mask WRITE setMask )
//    Q_PROPERTY( QString gateway READ gateway WRITE setGateway )
//    Q_PROPERTY( int time READ time WRITE setTime )
//    Q_PROPERTY( int waitingTime READ waitingTime WRITE setWaitingTime )
//    Q_PROPERTY( bool dynamic READ dynamic WRITE setDynamic )
public:
    enum { DHCPServer = 2 };
    dhcpServerProgramm();
    ~dhcpServerProgramm();

// Атрибуты
public:
    int id() const { return DHCPServer; }
    void setDevice(SmartDevice *s);
    void setServerName(QString n) { myServerName = n; }
    QString serverName() const { return myServerName; }
    virtual QString featureName() const { return "ds"; }
public:
    void showProperty();            
    void incTime();
    bool interrupt(int) { return false; }
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);

// Слоты
public slots:
    void execute(QByteArray data);
//    void checkInterface(QString port);

// Переменные
private:
    static int myServerCount;
    QString myServerName;
    QList<dhcpDemon*> myDemons;
    udpSocket *receiver;
};

#endif // DHCPSERVERPROGRAMM_H
