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
#ifndef TABLEARP_H
#define TABLEARP_H

#include <QDialog>

class QTableWidget;
class QPushButton;
class QLineEdit;
class QComboBox;
class QStringList;
class SmartDevice;
class ipEdit;
class macAddress;
class arpModel;

class tableArp : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(tableArp)
public:
    tableArp();
    void setDevice(SmartDevice *dev);
    void updateTable();
    void addItem(int r, int c, QString s);
private:
    SmartDevice *device;
    QTableWidget *table;
    QPushButton *btn_add;
    QPushButton *btn_del;
    QPushButton *btn_close;
    QLineEdit *le_mac;
    QComboBox *cb_port;
    QStringList s;
    QList<arpModel*> list;
    ipEdit *ip;
public slots:
    void addRecord();
    void deleteRecord();
    void checkSelection();
};

#endif // TABLEARP_H

