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
#include "sendellipse.h"
#include <QPen>
#include <QBrush>

SendEllipse::SendEllipse()
{
    setPen(QPen(Qt::lightGray));
    setBrush(QColor(255,128,0,128));
    setPos( 0 , -100);
    setZValue(1000);
    setRect(-10,-10,20,20);
}


void SendEllipse::chooseOneDevice()
{
    setPen(QPen(Qt::green));
    setBrush(QColor(0,128,0,128));
}

void SendEllipse::moving(QPointF p)
{
    setPos(p);
}
