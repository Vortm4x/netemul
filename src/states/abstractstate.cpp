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

AbstractState::AbstractState(MyCanvas *s)
{
    scene = s;
}

AbstractState* AbstractState::initialize(MyCanvas *s)
{
    return new EmptyState(s);
}

void AbstractState::goMove()
{
    AbstractState *oldState = scene->myState;
    scene->myState = new MoveState(scene);
    delete oldState;
}

void AbstractState::goInsert()
{
    AbstractState *oldState = scene->myState;
    scene->myState = new InsertState(scene);
    delete oldState;
}

void AbstractState::goCable()
{
    AbstractState *oldState = scene->myState;
    scene->myState = new CableState(scene);
    delete oldState;
}

void AbstractState::goText()
{
    AbstractState *oldState = scene->myState;
    scene->myState = new TextState(scene);
    delete oldState;
}

void AbstractState::goSend()
{
    AbstractState *oldState = scene->myState;
    scene->myState = new SendState(scene);
    delete oldState;
}

void AbstractState::goEmpty()
{
    AbstractState *oldState = scene->myState;
    scene->myState = new EmptyState(scene);
    delete oldState;
}


void AbstractState::goTo(int mode)
{
    switch ( mode ) {
        case move : goMove(); break;
        case insert : goInsert(); break;
        case cable : goCable(); break;
        case text : goText(); break;
        case send : goSend(); break;
    }
}
