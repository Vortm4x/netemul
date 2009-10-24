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

#include <QIcon>
#include "programmrep.h"
#include "dhcppacket.h"

static const int MINUTE = 60;

class interface;
class udpSocket;

struct interfaceState {
    enum { CS_NONE , CS_WAIT_VARIANT , CS_WAIT_RESPONSE , CS_ALL_RIGHT };
    int state;
    int xid;
    int time;
    ipAddress serverAddress;
    QString name;
    interface *adapter;
};

class dhcpClientProgramm : public programmRep
{
    Q_OBJECT
public:
    enum { DHCPClient = 51 ,CLIENT_SOCKET = 67, SERVER_SOCKET = 68  };
    dhcpClientProgramm();
    ~dhcpClientProgramm();
    bool interrupt(int) { return false; }
    void setDevice(smartDevice *s);
    void showProperty();
    void incTime();
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
    void observeInterface(const QString &name, bool b);
    QStringList interfacesList() const;
    QIcon isConnectSocketIcon(const QString &name) const;
    Qt::CheckState checkedState(const QString &name) const;
public slots:
    void deleteInterface(const QString name);
private slots:
    void processData(QByteArray data);
private:
    void sendDhcpMessage(dhcpPacket message, interfaceState *state);
    void sendRequest(const QString &name);
    void sendDiscover(const QString &name);
    void receiveOffer(dhcpPacket packet);
    void receiveAck(dhcpPacket packet);
    void restartSession( interfaceState *state);
    interfaceState* stateAt(const QString name);
    udpSocket *listener;
    QList<interfaceState*> states;
};

#endif // DHCPCLIENTPROGRAMM_H
