#include "deletecommand.h"
#include "mycanvas.h"
#include "cabledev.h"
#include "device.h"

deleteCommand::deleteCommand(myCanvas *s, QList<QGraphicsItem*> list)
{
    scene = s;
    foreach ( QGraphicsItem *i, list) {
        if ( scene->isDevice(i) ) extractDevices << qgraphicsitem_cast<device*>(i);
        else if ( i->type() == cableDev::Type ) extractCables << qgraphicsitem_cast<cableDev*>(i);
    }
    setText(QObject::tr("Delete"));
}

deleteCommand::~deleteCommand()
{
    if ( (!extractDevices.isEmpty() && isOnScene) || (!extractCables.isEmpty() && isOnScene) ) return;
    qDeleteAll(extractDevices);
    qDeleteAll(extractCables);
}

void deleteCommand::undo()
{
    foreach ( device *i, extractDevices ) scene->registerDevice(i);
    foreach ( cableDev *i, extractCables ) createConnection(i);
    isOnScene = true;
}

void deleteCommand::redo()
{
    foreach ( device *i, extractDevices ) scene->unregisterDevice(i);
    foreach ( cableDev *i, extractCables ) deleteConnection(i);
    isOnScene = false;
}

void deleteCommand::deleteConnection(cableDev *cable)
{
    cable->unregisterCable();
    scene->unregisterCable(cable);
}

void deleteCommand::createConnection(cableDev *cable)
{
    cable->registerCable();
    scene->registerCable(cable);
}
