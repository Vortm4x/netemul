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
#include <QtGui/QMessageBox>
#include "tcpsocket.h"
#include "ippacket.h"
#include "smartdevice.h"
#include "routemodel.h"
#include "appsetting.h"

TcpSocket::TcpSocket(SmartDevice *d,quint16 port) : AbstractSocket(d)
{
    myBindPort = port;
    isn = qrand()%Sequence;
    inputTime = 0;
    lastNum = 0;
    receiveIsn = 0;
    state = NONE;
    buffer.clear();
    sendingNum = 0;     
}

TcpSocket::~TcpSocket()
{
}

void TcpSocket::write(IpAddress a, quint16 p, QByteArray data)
{
    myBind = a;
    myReceiverPort = p;    
    buffer = data;
    setConnection();
}

void TcpSocket::setConnection()
{
    waitingTime = 0;
    timeout = 0;   
    tcpPacket t = createPacket(isn, 0, tcpPacket::SYN);
    sendMessage(t);
    state = WAIT_RESPONSE;
}

void TcpSocket::sendMessage(tcpPacket t) const
{
    ipPacket p;
    p.pack(t.toData());
    IpAddress gw;
    RouteRecord *r = dev->routeModel()->recordAt(myBind);
    if ( !r ) return;
    if ( r->gateway != r->out ) gw = r->gateway;
    p.setSender(r->out);
    p.setReceiver(myBind);
    p.setUpProtocol(SmartDevice::TCP);
    dev->ipToAdapter(r->out)->sendPacket(p,gw);
}

void TcpSocket::treatPacket(ipPacket p)
{
    tcpPacket tcp(p.unpack());
    if ( state == NONE && tcp.flag() != tcpPacket::SYN ) { emit imFinished(this); return; }

    if ( tcp.flag() == tcpPacket::ACK) { receiveAck(tcp); return; }

    if ( tcp.flag() == tcpPacket::NO_FLAGS ) {
        if ( state == R_WAIT ) {
            tcpPacket a = createPacket(0,0,tcpPacket::RST);
            sendMessage(a);
            emit imFinished(this);
            return;
        }
        inputTime = 0;
        lastNum = tcp.sequence();
        return;
    }
    if ( tcp.flag() == tcpPacket::FIN ) {
        lastNum = tcp.sequence();
        sendAck();
        emit receiveEnd();
        if ( m_isAutoDelete ) {
            emit imFinished(this);
        }
    }
    if ( tcp.flag() == (tcpPacket::SYN | tcpPacket::ACK) ) { receiveSynAck(tcp); return; }
    if ( tcp.flag() == tcpPacket::SYN ) {
        confirmConnection(p);
        return;
    }
    if ( tcp.flag() == tcpPacket::RST ) {
        error();
        return;
    }
}

void TcpSocket::receiveSynAck(tcpPacket t)
{
    if ( state != WAIT_RESPONSE ) return;
    timeout = 2*waitingTime;
    if ( t.ack() != isn ) return;
    isn = t.sequence();
    tcpPacket a = createPacket(0, t.sequence(), tcpPacket::ACK);
    sendMessage(a);
    sendWindow();
}

void TcpSocket::receiveAck(tcpPacket t)
{
    if ( state == R_WAIT ) { state = RECEIVE; return; }
    if ( state != WAIT_ACK ) return;
    panicTime = 0;
    sendingNum = 0;
    receiveIsn = t.ack();
    int r = (receiveIsn-sendIsn)*1024;
    buffer.remove(0,r);
    if ( buffer.isEmpty() ) {
        timeout = 0;
        emit writeEnd();
        if ( m_isAutoDelete ) {
            emit imFinished(this);
        }
        return;
    }
    sendWindow();
}

tcpPacket TcpSocket::createPacket(quint32 sequence, quint32 ack, quint8 flag) const
{
    tcpPacket t;
    t.setSender(myBindPort);
    t.setReceiver(myReceiverPort);
    t.setSequence( sequence );
    t.setAck(ack);
    t.setFlag(flag);
    t.setWindow(tcpPacket::Window);
    return t;
}

void TcpSocket::confirmConnection(ipPacket p)
{    
    tcpPacket tcp(p.unpack());
    myReceiverPort = tcp.sender();
    myBind = p.sender();
    tcpPacket t = createPacket(isn, tcp.sequence(), tcpPacket::SYN | tcpPacket::ACK);
    sendMessage(t);
    state = R_WAIT;
}

void TcpSocket::sendWindow()
{
    if ( sendingNum++ > appSetting::sendingNum() ) { error(); return; }
    panicTime = 0;
    sendIsn = isn;
    QByteArray data = buffer.left(tcpPacket::Window);
    int count = 0;
    int size = data.size();
    while ( size >= PACKET_SIZE ) {
        count++;        
        size -= PACKET_SIZE;
    }
    for ( int j = 0; j < count; j++ ) {
        tcpPacket t = createPacket(isn,0,tcpPacket::NO_FLAGS);
        if ( buffer.size()<=tcpPacket::Window && j==count-1) t.setFlag(tcpPacket::FIN);
        t.pack(data.left(PACKET_SIZE));
        data.remove(0,PACKET_SIZE);
        sendMessage(t);
        isn++;
    }
    state = WAIT_ACK;
}

void TcpSocket::sendAck()
{
     tcpPacket t = createPacket( 0, lastNum + 1, tcpPacket::ACK);
     sendMessage(t);
     inputTime = 0;
     lastNum = 0;
}

void TcpSocket::error()
{
    QMessageBox::critical(0,tr("Error"), tr("TCP: Data transmition error") );
    emit imFinished(this);
}

void TcpSocket::secondEvent()
{
    waitingTime++;
    if ( waitingTime > appSetting::waitingTime() && state == WAIT_RESPONSE ) {
        error();
        return;
    }
    if ( ++panicTime == timeout ) {
        sendWindow();
        return;
    }
    if ( lastNum && ++inputTime >= 2 ) sendAck();
}
