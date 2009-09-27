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
#include "abstractstate.h"
#include "emptystate.h"
#include "mycanvas.h"
#include "movestate.h"
#include "insertstate.h"
#include "cablestate.h"
#include "textstate.h"
#include "sendstate.h"

abstractState::abstractState(myCanvas *s)
{
    scene = s;
}

abstractState* abstractState::initialize(myCanvas *s)
{
    return new emptyState(s);
}

void abstractState::goMove()
{
    abstractState *oldState = scene->myState;
    scene->myState = new moveState(scene);
    delete oldState;
}

void abstractState::goInsert()
{
    abstractState *oldState = scene->myState;
    scene->myState = new insertState(scene);
    delete oldState;
}

void abstractState::goCable()
{
    abstractState *oldState = scene->myState;
    scene->myState = new cableState(scene);
    delete oldState;
}

void abstractState::goText()
{
    abstractState *oldState = scene->myState;
    scene->myState = new textState(scene);
    delete oldState;
}

void abstractState::goSend()
{
    abstractState *oldState = scene->myState;
    scene->myState = new sendState(scene);
    delete oldState;
}

void abstractState::goEmpty()
{
    abstractState *oldState = scene->myState;
    scene->myState = new emptyState(scene);
    delete oldState;
}


void abstractState::goTo(int mode)
{
    switch ( mode ) {
        case move : goMove(); break;
        case insert : goInsert(); break;
        case cable : goCable(); break;
        case text : goText(); break;
        case send : goSend(); break;
    }
}
