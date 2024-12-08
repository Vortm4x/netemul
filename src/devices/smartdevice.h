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
#ifndef SMARTDEVICE_H
#define SMARTDEVICE_H

#include <QVector>
#include <QIcon>
#include "deviceimpl.h"
#include "interface.h"
#include "program.h"

class RouteModel;
class ArpModel;
class RouteRecord;
class AbstractSocket;

/*!
  Интелектуальное устройство, абстрактный класс объединяющий в себе
  свойства компьютера и роутера.
*/
class SmartDevice : public DeviceImpl
{
    Q_OBJECT    
public:
    /*! Источники записи таблицы маршрутизации. */    
    enum { addNet = 100 , delNet = 101 };
    enum { UDP = 0 ,TCP = 1 };
    enum { USER_PORT = 7777 };
    /*! Значения для флага записи из таблицы маршрутизации. */    
    SmartDevice(QObject *parent);
    virtual ~SmartDevice();
    QString tableName() const { return tr("Routing Table"); }
    virtual void tableDialog();
    virtual void adapterDialog();
    virtual void programsDialog();
    virtual void arpDialog();
    virtual void showLogDialog(logDialog *log) const;

    virtual bool canManageInterface() const = 0;

    virtual Statistics deviceStatistics() const;
    bool isSmart() const { return true; }
    bool isReady() const;
    void checkReady() const;
    void checkTable();
    bool isBusy() const;
    bool isConnect() const;

    QStringList sockets() const;
    QStringList interfacesIp() const;
    QList<Cable*> cableList() const;
    DevicePort* findPortByName(const QString &name) const;
    FeaturesMap featuresList() const;
    void addInterface();
    bool isConnectSocket(const QString &socket) const { return adapter(socket)->isConnect(); }
    bool hasConnentSockets() const;
    QIcon isConnectSocketIcon(const QString &socket) const;
    QString socketName(const Cable *c) const;
    QString nameToIp(const QString &name) const { return adapter(name)->ip().toString(); }
    const Interface* adapter(const QString &s) const;

    Q_INVOKABLE void addInterface(Interface *in);

    Interface* ipToAdapter(const IpAddress &a);

    Interface* adapter(const QString &name);

    void sendMessage(const QString &a, int size ,int type);
    void treatPacket(IpPacket &p);
    void routePacket(IpPacket &p);
    void connectedNet(Interface *p);
    void deciSecondTimerEvent();
    void secondTimerEvent();
    int trafficDigit() const;
    IpAddress findInterfaceIp(IpAddress a);
    Program* programAt(const quint16 p) const;
    void removeProgram(Program *p);
    Q_INVOKABLE void addProgram( Program *p);
    bool sendInterrupt(int u);
    int socketsCount() const { return myInterfaces.count(); }
    ProgramList programs() { return myPrograms; }
    InterfaceVector interfaces() { return myInterfaces; }
//    void setInterfaces(const InterfaceVector &vector) { myInterfaces = vector; }
    void setRouter(bool n) { myRouter = n; }
    bool isRouter() const { return myRouter; }
    bool hasTable() const { return true; }
    bool hasProgram(int id);
    IpAddress gateway() const;
    QString gatewayString() const { return gateway().toString(); }
    AbstractSocket* openSocket(quint16 port , int type);
    RouteModel* routeModel() { return myRouteTable; }
    Q_INVOKABLE virtual void setRouteModel(RouteModel *model);
    QList<ArpModel*> arpModels();
public slots:
    void disposeSocket(AbstractSocket *socket);
    void addConnection(Cable*);
    void receivePacket(IpPacket p);
    void setCheckedSocket(const QString &str);
    void tableChanged(RouteRecord*,int n);
    void setIp(const QString &a, const QString &ip);
    QString ipaddress(const QString &name) const;
    void setMask(const QString &a, const QString &ip);
    void setGateway(const QString &str);
    virtual quint64 sendPacketCount(const QString &name) { return adapter(name)->countSendPacket(); }
    virtual quint64 receivePacketCount(const QString &name) { return adapter(name)->countRecPacket(); }
    virtual quint64 sendFrameCount(const QString &name) { return adapter(name)->countSendFrame(); }
    virtual quint64 receiveFrameCount(const QString &name) { return adapter(name)->countRecFrame(); }
signals:
    void interfaceDeleted(QString);
    void interfaceConnected(QString);
protected:    
    Interface* addInterface(const QString &name);
    void deleteInterface(const QString &name);
    bool myRouter;
    mutable bool myReady;
    mutable bool isDirty;
    InterfaceVector myInterfaces;
    ProgramList myPrograms; //!< Программы установленные на устройстве.
    QList<AbstractSocket*> mySockets; //!< Список установленных соединений.
    RouteModel *myRouteTable; //!< Таблица маршрутизации.
    virtual void write(QDataStream &stream) const;
    virtual void read(QDataStream &stream);
// FRIENDS:
public:
    friend class AdapterSetting;
};
//-------------------------------------------------------------------
/*!
  Модель данных для настроек адаптеров.
*/
class AdapterSetting {
public:
    AdapterSetting(SmartDevice *s) : sd(s) { }
    void setCurrent(int n);
    int current() const { return cur; }
    void resetStatics() { sd->myInterfaces.at(cur)->resetStatics(); }
    void connectedNet();
    int socketsCount() const { return sd->socketsCount(); }
    bool isConnect() const { return sd->myInterfaces.at(cur)->isConnect(); }
    QIcon isConnectSocketIcon() const { return sd->isConnectSocketIcon( sd->myInterfaces.at(cur)->name() ); }
    QString name() const { return sd->myInterfaces.at(cur)->name(); }
    QString mac() const { return sd->myInterfaces.at(cur)->mac().toString(); }
    QString ip() const { return sd->myInterfaces.at(cur)->ip().toString(); }
    QString mask() const { return sd->myInterfaces.at(cur)->mask().toString(); }
    void setMac(const QString &str) { sd->myInterfaces.at(cur)->setMac(str); }
    void setIp(const QString &str) { sd->myInterfaces.at(cur)->setIp(str); }
    void setMask(const QString &str) { sd->myInterfaces.at(cur)->setMask(str); }
    void setCheckedSocket(const QString &str) { sd->setCheckedSocket(str); }
    QString statics() const { return sd->myInterfaces.at(cur)->statisticsString(); }
    void sendArpRequest(IpAddress a) { sd->myInterfaces.at(cur)->sendArpRequest(a); }
    bool hasDhcpClient() const { return sd->hasProgram( Program::DHCPClient ); }
    bool isUnderDhcpControl() const;
    void setUnderDhcpControl(bool isUnder);
    bool canManageInterface() const { return sd->canManageInterface(); }
    void addInterface() { sd->addInterface(); }
    void deleteInterface(const QString &name) { sd->deleteInterface(name); }
private:
    IpAddress oldIp;
    IpAddress oldMask;
    int cur;
    SmartDevice *sd;
};
//-------------------------------------------------------------

class SmartSetting  : public DeviceSetting {
public:
    SmartSetting(SmartDevice *d) : DeviceSetting(d) , sd(d) { }
    int socketsCount() const { return sd->socketsCount(); }
    bool isRouter() const { return sd->isRouter(); }
    void setRouter(bool b) { sd->setRouter(b); }
private:
    SmartDevice *sd;
};


#endif // SMARTDEVICE_H
