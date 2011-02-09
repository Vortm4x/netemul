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
#ifndef SCENECONTROL_H
#define SCENECONTROL_H

#include <QtCore/QObject>
#include <QtGui/QIcon>

class MyCanvas;
class logDialog;

class SceneControl : public QObject
{
    Q_OBJECT
public:
    SceneControl(QObject *parent,MyCanvas *s);
    bool isSelect() const;
    QString tableName() const;
    QString note() const;
    QStringList sockets() const;
    QIcon tableIcon() const;
    QString deviceName() const;
private:
    MyCanvas *scene;
public slots:
    void observeSelection();
    void propertyDialog() const;
    void tableDialog() const;
    void adapterDialog() const;
    void programmsDialog() const;
    void arpDialog() const;
    void showLogDialog(logDialog *log) const;
    void showDesignerDialog() const;
    void showDeviceNoteDialog() const;
    void showVirtualNetworkDialog() const;
signals:
    void selectOneDevice(bool);
    void selectTableDevice(bool);
    void selectSmartDevice(bool);
    void selectComputer(bool);
    void selectVirtualNetworkDevice(bool);
};

#endif // SCENECONTROL_H
