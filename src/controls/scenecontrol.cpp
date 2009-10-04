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
#include "scenecontrol.h"
#include "device.h"
#include "mycanvas.h"

sceneControl::sceneControl(QObject *parent , myCanvas *s ) : QObject(parent) , scene(s)
{
    connect( scene , SIGNAL(selectionChanged()) , SLOT(observeSelection()) );
}

void sceneControl::observeSelection()
{
    if ( device *t = scene->oneSelectedDevice() ) {
        emit selectTableDevice( t->hasTable() );
        emit selectOneDevice(true);
        emit selectSmartDevice( t->isSmart() );
        emit selectComputer( t->isCanSend() );
    }
    else {
        emit selectSmartDevice(false);
        emit selectOneDevice(false);
        emit selectTableDevice(false);
    }
}

QString sceneControl::tableName() const
{
    return scene->oneSelectedDevice()->tableName();
}

void sceneControl::propertyDialog() const
{
    device *t = scene->oneSelectedDevice();
    t->dialog();
}

void sceneControl::tableDialog() const
{
    device *t = scene->oneSelectedDevice();
    t->tableDialog();
}

void sceneControl::adapterDialog() const
{
    device *t = scene->oneSelectedDevice();
    t->adapterDialog();
}

void sceneControl::programmsDialog() const
{
    device *t = scene->oneSelectedDevice();
    t->programmsDialog();
}

void sceneControl::arpDialog() const
{
    device *t = scene->oneSelectedDevice();
    t->arpDialog();
}

void sceneControl::showLogDialog(logDialog *log) const
{
    device *t = scene->oneSelectedDevice();
    t->showLogDialog(log);
}

void sceneControl::showDesignerDialog() const
{
    device *t = scene->oneSelectedDevice();
    t->showDesignerDialog();
}

bool sceneControl::isSelect() const
{
    if ( scene->oneSelectedDevice() ) return true;
    return false;
}

QString sceneControl::note() const
{
    device *t = scene->oneSelectedDevice();
    return t->toolTip();
}

QIcon sceneControl::tableIcon() const
{
    device *t = scene->oneSelectedDevice();
    if ( !t->hasTable() ) return QIcon();
    if ( t->isSmart() ) return QIcon(":/im/images/table_route.png");
    else return QIcon(":/im/images/table_arp.png");
}

QString sceneControl::deviceName() const
{
    device *t = scene->oneSelectedDevice();
    return t->deviceName();
}

QStringList sceneControl::sockets() const
{
    device *t = scene->oneSelectedDevice();
    return t->sockets();
}
