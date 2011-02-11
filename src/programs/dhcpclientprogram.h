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
#ifndef DHCPCLIENTPROGRAMM_H
#define DHCPCLIENTPROGRAMM_H

#include <QMetaType>
#include <QIcon>
#include "program.h"
#include "dhcppacket.h"

static const int MINUTE = 60;
static const int REPEAT_COUNT = 3;

class Interface;
class AbstractSocket;

struct InterfaceState {
    enum { CS_NONE , CS_WAIT_VARIANT , CS_WAIT_RESPONSE , CS_ALL_RIGHT };
    int state;
    int xid;
    int time;
    qint8 count;
    IpAddress serverAddress;
    IpAddress lastIp;
    QString name;
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
};
typedef QList<InterfaceState*> InterfaceStateList;

//xid << time << serverAddress << lastIp << name;

class InterfaceStateObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY( int xid READ xid WRITE setXid )
    Q_PROPERTY( QString serverAddress READ serverAddress WRITE setServerAddress )
    Q_PROPERTY( QString lastIp READ lastIp WRITE setLastIp )
    Q_PROPERTY( QString name READ name WRITE setName )
public:
    InterfaceStateObject(InterfaceState *s) { st = s; }
    InterfaceStateObject(QObject *parent = 0) : QObject(parent) { st = new InterfaceState(); }

    InterfaceState* object() { return st; }

    int xid() const { return st->xid; }
    QString serverAddress() const { return st->serverAddress.toString(); }
    QString lastIp() const { return st->lastIp.toString(); }
    QString name() const { return st->name; }

    void setXid( int x ) { st->xid = x; }
    void setServerAddress( const QString &x ) { st->serverAddress.setIp(x); }
    void setLastIp( const QString &x ) { st->lastIp.setIp(x); }
    void setName( const QString &x ) { st->name = x; }

private:
    InterfaceState *st;
};


class DhcpClientProgram : public Program
{
    Q_OBJECT
    Q_PROPERTY( int offerTime READ offerTime WRITE setOfferTime )
    Q_PROPERTY( QVariantList states READ statesObjectList )
public:
    enum { DHCPClient = 1 ,CLIENT_SOCKET = 67, SERVER_SOCKET = 68  };
    DhcpClientProgram(QObject *parent = 0);
    ~DhcpClientProgram();
    int id() const { return DHCPClient; }
    bool interrupt(int) { return false; }
    void setDevice(SmartDevice *s);
    void showProperty();
    void incTime();
    bool isUnderDhcpControl(const QString name) const;
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
    void observeInterface(const QString &name, bool b);
    QStringList interfacesList() const;
    QIcon isConnectSocketIcon(const QString &name) const;
    Qt::CheckState checkedState(const QString &name) const;
    virtual QString featureName() const { return "dc"; }
public slots:
    void deleteInterface(const QString name);
private slots:
    void processData(QByteArray data);
    void onDetectEqualIp();
//Property
public:
    Q_INVOKABLE void addInterfaceStateObject(InterfaceStateObject *obj);
    void setOfferTime(int time) { myOfferTime = time; }
    int offerTime() const { return myOfferTime; }
    InterfaceStateList states() { return myStates; }
    QVariantList statesObjectList() const;

private:        
    InterfaceStateList myStates;
    int myOfferTime;

private:
    void sendDhcpMessage(DhcpPacket message, InterfaceState *state);
    void sendRequest(const QString &name);
    void sendDiscover(const QString &name);
    void sendDecLine(const QString &name);
    void receiveOffer(DhcpPacket packet);
    void receiveAck(DhcpPacket packet);
    void restartSession( InterfaceState *session);
    InterfaceState* stateAt(const QString name);
    void resetClient( InterfaceState *session);
    AbstractSocket *listener;
};

#endif // DHCPCLIENTPROGRAMM_H
