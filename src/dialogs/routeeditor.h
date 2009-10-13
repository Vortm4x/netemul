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
#ifndef ROUTEEDITOR_H
#define ROUTEEDITOR_H

#include <QDialog>
#include <QModelIndex>
class QTableView;
class QPushButton;
class QSpinBox;
class QComboBox;

class ipEdit;
class smartDevice;
class routeModel;

/*!
  Реализует отображение таблицы маршрутизации.
*/
class routeEditor : public QDialog
{
    Q_OBJECT
public:
    routeEditor(smartDevice *s);
    ~routeEditor();
private:
    void readSetting();
    void writeSetting() const;
    QStringList filterConnectedSocket(QStringList list);
    QStringList suffixList;
    QStringList ipList;
    ipEdit *ip_dest;
    ipEdit *ip_mask;
    ipEdit *ip_gateway;
    QComboBox *cb_out;
    QSpinBox *sp_metr;
    QPushButton *btn_add;
    QPushButton *btn_remove;
    QPushButton *btn_close;
    QTableView *table;
    routeModel *model;
    smartDevice *dev;
private slots:
    void selectAdapter(int number);
    void addRecord();
    void deleteRecord();
    void checkSelection(QModelIndex curr);
};
//--------------------------------------------------

#endif // ROUTEEDITOR_H
