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
#ifndef SENDDIALOG_H
#define SENDDIALOG_H

#include <QDialog>
#include <QMap>
#include "macaddress.h"
#include "ipaddress.h"
class QListWidget;
class QListWidgetItem;
class QPushButton;
class QLabel;
class QSlider;
class QSpinBox;
class QCheckBox;
class QRadioButton;
class QVBoxLayout;
class DeviceImpl;
class Device;
class DevicePort;
class Interface;

class sendDialog : public QDialog
{
    Q_OBJECT
public:
    enum aim { sender = 0 , receiver = 1 };
    enum { UDP = 0 ,TCP = 1 };
    sendDialog(aim cur,Device *t);
    void prepare();
    QString dest() const { return myDest; }
    bool broadcast() const { return myBroadcast; }
    int messageSize() const { return mySize; }
    int protocol() const { return myProtocol; }

public slots:
    void checkSelected(int cur);
    void checkAccept();
private:
    aim myState;
    DeviceImpl *myDevice;
    QString myDest;
    int mySize;
    int myProtocol;
    bool myBroadcast;
    QSpinBox *sizeBox;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QRadioButton *rtn_udp;
    QRadioButton *rtn_tcp;
    QSlider *sizeSlider;
    QLabel *caption;
    QLabel *sizeCaption;
    QListWidget *list;
//    QCheckBox *check;
    QVBoxLayout *all;
};

#endif // SENDDIALOG_H
