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
