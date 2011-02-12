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
#ifndef PROGRAMMDIALOG_H
#define PROGRAMMDIALOG_H

#include "ui_programmdialog.h"

class SmartDevice;

/*!
  Реализует диалог установленных пограмм устройства.
*/
class ProgramDialog : public QDialog, private Ui::ProgramDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(ProgramDialog)
public:
    ProgramDialog(QWidget *parent = 0);
    void setDevice( SmartDevice *d );
protected:
    void updateList();
    void changeEvent(QEvent *e);
private:
    SmartDevice *s; //!< Указатель на устройство.
public slots:
    void apply();
private slots:
    void stateChanged(QListWidgetItem *item);
    void programmChanged();
    void settings();
    void add();
    void remove();
};

#endif // PROGRAMMDIALOG_H
