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
#ifndef DELETECOMMAND_H
#define DELETECOMMAND_H

#include <QUndoCommand>
#include <QSet>

class QGraphicsItem;
class MyCanvas;
class Cable;
class Device;
class TextItem;

class DeleteCommand : public QUndoCommand
{
public:
    DeleteCommand(MyCanvas *s, QList<QGraphicsItem*> list);
    ~DeleteCommand();
    void undo();
    void redo();
    void deleteConnection(Cable *cable);
    void createConnection(Cable *cable);
private:
    MyCanvas *scene;
    QSet<Cable*> extractCables;
    QList<Device*> extractDevices;
    QList<TextItem*> extractText;
    bool isOnScene;
};

#endif // DELETECOMMAND_H
