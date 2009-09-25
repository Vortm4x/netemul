#include "addcommand.h"
#include "mycanvas.h"

addCommand::addCommand(myCanvas *s, QPointF point , int type)
{
    scene = s;
    myPoint = point;
    myType = type;
    setText(QObject::tr("Add"));
}

void addCommand::undo()
{
    scene->removeDevice( scene->deviceInPoint(myPoint) );
}

void addCommand::redo()
{
    scene->addDeviceOnScene( myPoint, myType );
}
