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
#ifndef ADAPTERPROPERTY_H
#define ADAPTERPROPERTY_H

#include "dialogtemplate.h"

class QTabBar;
class QLineEdit;
class QCheckBox;
class QLabel;
class QPushButton;

class devicePort;
class ipEdit;
class adapterSetting;

/**
  Класс диалога настройки адаптеров.
*/
class adapterProperty : public dialogTemplate
{
    Q_OBJECT
    Q_DISABLE_COPY(adapterProperty)
public:
    adapterProperty(adapterSetting *s);
    ~adapterProperty();
private slots:
    void updateTab(int n);
    void addInterface();
    void deleteInterface();
    void reset();
    void changeTab(int n);
    void apply();
private:
    adapterSetting *sd;
    QLabel *lb_statics;
    QTabBar *tab_interfaces;
    QLineEdit *le_name;
    QLineEdit *le_mac;
    ipEdit *le_ip;
    ipEdit *le_mask;
    QPushButton *btn_add;
    QPushButton *btn_del;
};

#endif // ADAPTERPROPERTY_H
