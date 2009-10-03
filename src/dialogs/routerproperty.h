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
#ifndef ROUTERPROPERTY_H
#define ROUTERPROPERTY_H

#include "dialogtemplate.h"

class routerSetting;
class QCheckBox;
class QGroupBox;
class QComboBox;
class QPlainTextEdit;

class routerProperty : public dialogTemplate
{
    Q_OBJECT
    Q_DISABLE_COPY(routerProperty)
public:
    routerProperty();
    void setRouter(routerSetting *r);
private:
    routerSetting *rt;
    QPlainTextEdit *te_text;
    QCheckBox *cb_route;
    QComboBox *cb_count;
public slots:
    void apply();
};

#endif // ROUTERPROPERTY_H
