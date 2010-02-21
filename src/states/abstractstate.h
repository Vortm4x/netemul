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
#ifndef ABSTRACTSTATE_H
#define ABSTRACTSTATE_H

class MyCanvas;
class QGraphicsSceneMouseEvent;

class abstractState
{
public:
    abstractState(MyCanvas *s);
    enum { move = 0 , cable = 1 , insert = 2 , send = 6 , text = 8};
    virtual ~abstractState() { }
    virtual void mouseMove(QGraphicsSceneMouseEvent*) = 0;
    virtual void mousePress(QGraphicsSceneMouseEvent*) = 0;
    virtual void mouseRelease(QGraphicsSceneMouseEvent*) = 0;
    virtual void goMove();
    virtual void goInsert();
    virtual void goCable();
    virtual void goText();
    virtual void goSend();
    virtual void goEmpty();
    virtual void hideState() { }
    static abstractState* initialize(MyCanvas *s);
    void goTo(int mode);
protected:
    MyCanvas *scene;
};

#endif // ABSTRACTSTATE_H
