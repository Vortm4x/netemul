#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H

#include <QUndoCommand>
#include "mycanvas.h"

typedef QMap<QGraphicsItem*,QPointF> mapWithCoords;

class moveCommand : public QUndoCommand
{
public:
    moveCommand(myCanvas *s,mapWithCoords o, mapWithCoords r);
    void undo();
    void redo();
private:
    myCanvas *scene;
    mapWithCoords old;
    mapWithCoords recent;
};

#endif // MOVECOMMAND_H
