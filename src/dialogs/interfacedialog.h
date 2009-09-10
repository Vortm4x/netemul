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
#ifndef INTERFACEDIALOG_H
#define INTERFACEDIALOG_H

#include <QDialog>

class interface;


class QListWidget;
class QPushButton;
class QLabel;


class interfaceDialog : public QDialog
{
    Q_OBJECT
public:
    interfaceDialog();
    int  result() {  return res;  }
public slots:
    void checkAccept();
    void changeItem(int n);
private:
    int res;

    QLabel *picture;
    QLabel *speed;

    QListWidget *standarts;
    QPushButton *addEditButton;
    QPushButton *cancelButton;
};

#endif // INTERFACEDIALOG_H
