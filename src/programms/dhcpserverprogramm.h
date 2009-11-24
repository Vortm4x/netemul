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
#include "ipaddress.h"
#include "macaddress.h"

class udpSocket;
class dhcpPacket;
class interface;
class staticDhcpRecord;
class dhcpServerModel;


struct clientState {
    int xid;
    int state;
    macAddress mac;
    ipAddress ip;
    ipAddress mask;
    ipAddress gateway;
    int time;
    int requestTimer;
    enum { WAIT_REQUEST = 0, IN_USE = 1, DECLINE = 2 };
    clientState(staticDhcpRecord *rec);
    clientState() { }
};

class dhcpServerProgramm : public programmRep
{
    Q_OBJECT
    Q_PROPERTY( QString interfaceName READ interfaceName WRITE setInterfaceName )
    Q_PROPERTY( QString beginIp READ beginIp WRITE setBeginIp )
    Q_PROPERTY( QString endIp READ endIp WRITE setEndIp )
    Q_PROPERTY( QString mask READ mask WRITE setMask )
    Q_PROPERTY( QString gateway READ gateway WRITE setGateway )
    Q_PROPERTY( int time READ time WRITE setTime )
    Q_PROPERTY( int waitingTime READ waitingTime WRITE setWaitingTime )
    Q_PROPERTY( bool dynamic READ dynamic WRITE setDynamic )
public:
    enum { DHCPServer = 2 , CLIENT_SOCKET = 67 , SERVER_SOCKET = 68 };
    dhcpServerProgramm();
    ~dhcpServerProgramm();
// Атрибуты
public:
    int id() const { return DHCPServer; }
    void setDevice(smartDevice *s);
    void setInterfaceName( QString inter );
    void setBeginIp(QString ip) { myBeginIp.setIp(ip); }
    void setEndIp(QString ip) { myEndIp.setIp(ip); }
    void setMask(QString m) { myMask.setIp(m); }
    void setGateway(QString g) { myGateway.setIp(g); }
    void setTime(int t) { myTime = t; }
    void setWaitingTime(int t) { myWaitingTime = t; }
    void setDynamic(bool b) { myDynamic = b; }
    QString interfaceName() const { return myInterface; }
    QString beginIp() const { return myBeginIp.toString(); }
    QString endIp() const { return myEndIp.toString(); }
    QString mask() const { return myMask.toString(); }
    QString gateway() const { return myGateway.toString(); }
    int time() const { return myTime; }
    int waitingTime() const { return myWaitingTime; }
    bool dynamic() const { return myDynamic; }
    dhcpServerModel* dhcpModel() { return myDhcpModel; }
    virtual QString featureName() const { return "DS"; }
public:
    void showProperty();        
    ipAddress giveDynamicIp() const;
    void incTime();
    bool interrupt(int) { return false; }
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
    void writeXml(sceneXmlWriter &stream) const;
    void readXml(sceneXmlReader &stream);
// Слоты
public slots:
    void execute(QByteArray data);
    void checkInterface(QString port);
// Обработка пакетов
private:
    void executeDiscover(dhcpPacket packet);
    void executeRequest(dhcpPacket packet);
    void executeDecline(dhcpPacket packet);
// Функции создания и отправки ответа
private:
    void makeAnswer(clientState* client, int type);
    void sendDhcp(dhcpPacket packet) const;
    dhcpPacket createDhcpPacket( clientState *client, int state ) const;
// Функции выбора и нахождения записи клиента
private:
    clientState* chooseStatic(dhcpPacket packet);
    clientState* chooseDynamic(dhcpPacket packet);
    clientState* findClient( int xid ) const;
    clientState* findClient(ipAddress ip) const;

// Переменные
private:
    QList<clientState*> clients;
    QString myInterface;
    udpSocket *receiver;
    dhcpServerModel *myDhcpModel;
    ipAddress myBeginIp;
    ipAddress myEndIp;
    ipAddress myMask;
    ipAddress myGateway;
    int myTime;
    int myWaitingTime;
    bool myDynamic;
};

#endif // DHCPSERVERPROGRAMM_H
