#include "movecommand.h"

moveCommand::moveCommand(myCanvas *s,mapWithCoords o,mapWithCoords r)
{
    scene = s;
    old = o;
    recent = r;
    setText(QObject::tr("Move"));
}

void moveCommand::undo()
{
    scene->putItems(old);
}

void moveCommand::redo()
{
    scene->putItems(recent);
}
