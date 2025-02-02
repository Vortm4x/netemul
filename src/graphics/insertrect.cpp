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
#include "insertrect.h"
#include <QPen>
#include <QBrush>

InsertRect::InsertRect()
{
    setPos( 0, -50 );
    setPen(QPen(Qt::red));
    setBrush(QColor( 128 , 0 , 0 , 64));
    //setZValue(1000.0);
    setRect( -20 , -20 , 40 ,40);
}

void InsertRect::moving(QPointF p)
{
    if ( collidingItems().isEmpty() ) {
        setPen(QPen(Qt::blue));
        setBrush( QColor( 0 , 0 , 128 , 64) );
    }
    else {
        setPen(QPen(Qt::red));
        setBrush( QColor( 128 , 0 , 0 , 64));
    }
    setPos(p);
}

