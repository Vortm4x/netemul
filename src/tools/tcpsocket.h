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
#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include "abstractsocket.h"
#include "tcppacket.h"

class tcpSocket : public abstractSocket
{
    Q_OBJECT
public:
    enum { Sequence = 5000, PACKET_SIZE = 1024 };
    enum { NONE = 0, WAIT_RESPONSE = 1, WAIT_ACK = 2, R_WAIT = 3, RECEIVE = 4 };
    tcpSocket() { }
    tcpSocket(SmartDevice *d,quint16 port);
    ~tcpSocket();
    void setConnection();
    void treatPacket(ipPacket p);
    void confirmConnection(ipPacket p);
    void secondEvent();
    void write(IpAddress a, quint16 p, QByteArray data);
    bool isBusy() const { return !buffer.isEmpty(); }
signals:
    void writeEnd();
    void receiveEnd();
private:
    void sendMessage(tcpPacket t) const;
    void sendWindow();
    void sendAck();
    void receiveSynAck(tcpPacket t);
    void receiveAck(tcpPacket t);
    void error();
    tcpPacket createPacket( quint32 sequence, quint32 ack, quint8 flag) const;
    int state;
    quint16 myReceiverPort;
    quint32 sendIsn;
    quint32 receiveIsn;
    quint32 isn;
    int timeout;
    int inputTime;
    int panicTime;
    int lastNum;
    int sendingNum; // Число повторных отправлений
    int waitingTime;
    QByteArray buffer;
};

#endif // TCPSOCKET_H
