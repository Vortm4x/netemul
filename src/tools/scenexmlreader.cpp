#include "scenexmlreader.h"
#include "mycanvas.h"
#include "device.h"

#include <QtDebug>

sceneXmlReader::sceneXmlReader(myCanvas *scene) : myScene(scene)
{

}

void sceneXmlReader::readScene(QIODevice *dev)
{
    setDevice(dev);
    while ( !atEnd() ) {
        readNext();
        if ( !isStartElement() ) continue;
        if ( name() == "netemul") {
                readNetemul();
        }
        else {
            raiseError(QObject::tr("The file is not a NetEmul file."));
        }
    }
}

void sceneXmlReader::readUnknownElement()
{
    if ( !isStartElement() ) {
        qDebug() << name() << tokenString() ;
        qFatal("panic");
    }

    while (!atEnd()) {
        readNext();

        if (isEndElement())
            break;

        if (isStartElement())
            readUnknownElement();
    }
}

void sceneXmlReader::readNetemul()
{
    while ( !atEnd() ) {
        readNext();
        if ( isEndElement() ) break;
        if ( name() == "device" ) {
           device::device *item = new ::device(*this);
           item->setMenu(myScene->itemMenu() );
           myScene->registerDevice(item);
        }
        else if ( name() == "textitem" ) {
            QPointF p;
            p.setX( attributes().value("x").toString().toDouble() );
            p.setY( attributes().value("y").toString().toDouble() );
            myScene->createTextItem( p , readElementText() );
        }
    }
}

