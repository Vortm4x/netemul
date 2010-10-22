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
#ifndef DHCPSERVERPROPERTY_H
#define DHCPSERVERPROPERTY_H

#include <ui_dhcpserverproperty.h>
#include "ipedit.h"

class DhcpServerProgram;
class dhcpServerModel;
class SmartDevice;
class macAddressDelegate;
class ipAddressDelegate;

class dhcpServerProperty : public QDialog, private Ui::dhspServerProperty
{
    Q_OBJECT
public:
    dhcpServerProperty(SmartDevice *dev,QWidget *parent = 0);
    ~dhcpServerProperty();
    void setProgramm( DhcpServerProgram *prog );
public slots:
    void addRecord();
    void deleteRecord();
    void changeState(bool);
    void apply();
protected:
    void changeEvent(QEvent *e);
private:
    macAddressDelegate *macDelegate;
    ipAddressDelegate *ipDelegate;
    DhcpServerProgram *myProgramm;
    dhcpServerModel *myModel;
    SmartDevice *device;
};

#endif // DHCPSERVERPROPERTY_H
