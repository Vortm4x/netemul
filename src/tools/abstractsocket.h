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
#ifndef ABSTRACTSOCKET_H
#define ABSTRACTSOCKET_H

#include "ippacket.h"

class SmartDevice;

class AbstractSocket : public QObject
{
    Q_OBJECT
public:
    AbstractSocket() { dev = 0; }
    AbstractSocket(SmartDevice *d);
    virtual ~AbstractSocket();
    bool isOurData(IpAddress address, quint16 port);
    virtual bool isBusy() const { return false; }
    void setBind(IpAddress address) { myBind = address; }
    virtual void treatPacket(ipPacket) { }
    virtual void secondEvent() { }
    virtual void write(IpAddress, quint16, QByteArray) { }

    void setAutoDelete(bool f) { m_isAutoDelete = f; }
    bool autoDelete() const { return m_isAutoDelete; }
signals:
    void readyRead(QByteArray);
    void imFinished(AbstractSocket*);
protected:    
    quint16 myBindPort;
    IpAddress myBind;
    SmartDevice *dev;
    bool m_isAutoDelete;
};

#endif // ABSTRACTSOCKET_H
