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
#ifndef HUBPROPERTY_H
#define HUBPROPERTY_H

#include "dialogtemplate.h"

class QComboBox;
class QLineEdit;
class QLabel;
class QPushButton;

class ipEdit;
class hubSetting;

class hubProperty : public dialogTemplate
{
Q_OBJECT
public:
    hubProperty();
    ~hubProperty();
    void setHub(hubSetting *s);
private:
    hubSetting *st;
    QComboBox *cb_count;
    QLabel *lb_statics;
    QLabel *lb_mac;
    QLabel *lb_conflict;
    QLineEdit *le_mac;
public slots:
    void apply();
private slots:
    void reset();
};

#endif // HUBPROPERTY_H
