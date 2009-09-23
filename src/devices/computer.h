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
#ifndef COMPUTER_H
#define COMPUTER_H

#include "smartdevice.h"
#include "deviceport.h"
#include <QDataStream>

class devicePort;
/*!
  Реализует компьютер.
*/
class computer : public smartDevice
{
    Q_OBJECT
public:
    enum { compDev = 3 };
    int type() const { return compDev; }
    computer(int c = 0);
    ~computer() {  }

    void dialog();

    QString deviceName() const { return "comp"; }
    bool canManageInterface() const { return true; }
    bool isCanSend() const { return true; }
protected:
    void write(QDataStream &stream) const { stream << compDev; smartDevice::write(stream);}
    void read(QDataStream &stream) { smartDevice::read(stream); }
};
//---------------------------------------------

class computerSetting : public smartSetting {
public:
    computerSetting( computer *c ) : smartSetting(c) , cp(c) { }
    QString gateway() const { return cp->gateway().toString(); }
    void setGateway(const QString &str) { cp->setGateway(str); }
private:
    computer *cp;
};
#endif // COMPUTER_H
