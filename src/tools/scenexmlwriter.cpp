#include "scenexmlwriter.h"
#include "mycanvas.h"
#include "device.h"

sceneXmlWriter::sceneXmlWriter(myCanvas *scene) : myScene(scene)
{
    setAutoFormatting(true);
}

void sceneXmlWriter::writeScene(QIODevice *dev)
{
    setDevice(dev);
    writeStartDocument();
    writeStartElement("netemul");
    writeAttribute("version",QCoreApplication::applicationVersion() );
    foreach ( device::device *i , myScene->devices() )
        i->writeXml(*this);
    writeEndDocument();
}
