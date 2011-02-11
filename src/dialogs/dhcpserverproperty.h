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
#include "dhcpserverprogram.h"

class DhcpServerModel;
class MacAddressDelegate;
class IpAddressDelegate;

class DhcpServerProperty : public QDialog, private Ui::dhspServerProperty
{
    Q_OBJECT
public:
    DhcpServerProperty(DhcpServerSetting *setting, QWidget *parent = 0);
    ~DhcpServerProperty();
    void setProgram( DhcpServerProgram *prog );

protected:
    void changeEvent(QEvent *e);

private slots:
    void onCurrentChanged(int index);
    void onTurnedOnOff(bool b);
    void addRecord();
    void deleteRecord();
    void changeState(bool);
    void apply();

private: // Functions
    void clearForm();


private:
    MacAddressDelegate *macDelegate;
    IpAddressDelegate *ipDelegate;
    DhcpServerModel *myModel;
    DhcpServerSetting* mySetting;
    DhcpDaemon* myDaemon;
    QMap<int, Interface*> *myPortMap;

    QTabBar *port_tb;
};

#endif // DHCPSERVERPROPERTY_H
