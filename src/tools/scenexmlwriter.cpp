#include "scenexmlwriter.h"
#include "mycanvas.h"
#include "device.h"

#include <QMetaObject>

sceneXmlWriter::sceneXmlWriter(MyCanvas *scene) : myScene(scene)
{
    setAutoFormatting(true);
}

void sceneXmlWriter::writeEntity(const QObject *entity)
{
    const QMetaObject *t = entity->metaObject();
    writeStartElement(t->className());
    for ( int j = 0 ; j< t->propertyCount() ; j++ ) {
        QMetaProperty p = t->property(j);
        writeAttribute( p.name() , p.read(entity).toString() );
    }
    foreach ( const QObject *c , entity->children() ) {
        writeEntity(c);
    }
    writeEndElement();
}

void sceneXmlWriter::writeScene(QIODevice *dev)
{
    setDevice(dev);
    writeStartDocument();
    writeStartElement("netemul");
    writeAttribute("version",QCoreApplication::applicationVersion() );
    foreach ( Device *i , myScene->devices() ) {
        writeEntity(i);
    }
    foreach ( textItem *i , myScene->textItems() ) {        
        const QMetaObject *t = i->metaObject();
        writeStartElement(t->className());
        for ( int j = 0 ; j< t->propertyCount() ; j++ ) {
            QMetaProperty p = t->property(j);
            writeAttribute( p.name() , p.read(i).toString() );
        }
        writeEndElement();
    }
    writeEndDocument();
}
