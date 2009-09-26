#include "addcablecommand.h"
#include "mycanvas.h"
#include "cabledev.h"

addCableCommand::addCableCommand(myCanvas *s, cableDev *c)
{
    scene = s;
    cable = c;
}

addCableCommand::~addCableCommand()
{
    if ( !cable->scene() ) delete cable;
}

void addCableCommand::undo()
{
    cable->unregisterCable();
    scene->unregisterCable(cable);
}

void addCableCommand::redo()
{
    cable->registerCable();
    scene->registerCable(cable);
}
