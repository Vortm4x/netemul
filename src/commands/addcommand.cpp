#include "device.h"
#include "addcommand.h"
#include "mycanvas.h"

addCommand::addCommand(myCanvas *s, QPointF point , int type)
{
    scene = s;
    myDevice = 0;
    myPoint = point;
    myType = type;
    setText(QObject::tr("Add"));
}

addCommand::~addCommand()
{
    if ( !isOnScene ) delete myDevice;
}

void addCommand::undo()
{
    scene->unregisterDevice(myDevice);
    isOnScene = false;
}

void addCommand::redo()
{
    if ( myDevice ) scene->registerDevice(myDevice);
    else myDevice = scene->addDeviceOnScene( myPoint, myType );
    isOnScene = true;
}
