#include "scenexmlwriter.h"
#include "mycanvas.h"
#include "device.h"

#include <QMetaObject>

SceneXmlWriter::SceneXmlWriter(MyCanvas *scene) : myScene(scene)
{
    setAutoFormatting(true);
}

void SceneXmlWriter::writeEntity(const QObject *entity)
{
    const QMetaObject *t = entity->metaObject();
    writeStartElement(t->className());
    QVariantList list;
    for ( int j = 0 ; j< t->propertyCount() ; j++ ) {        
        QMetaProperty p = t->property(j);
        if ( strcmp(p.name(),"objectName") != 0 ) {
            QVariant v = p.read(entity);
            if ( p.type() == QVariant::List) {
                list.append( p.read(entity).toList() );
            } else if ( v.type() == QMetaType::QObjectStar ) {
                list.append(v);
            } else {
                writeAttribute( p.name() , p.read(entity).toString() );
            }
        }
    }    
    if ( !list.isEmpty() ) {
        foreach ( const QVariant &v , list ) {
            QObject *obj = v.value<QObject*>();
            writeEntity(obj);
            delete obj;
        }
    }
    foreach ( const QObject *c , entity->children() ) {
        writeEntity(c);
    }
    writeEndElement();
}

void SceneXmlWriter::writeScene(QIODevice *dev)
{
    setDevice(dev);
    writeStartDocument();
    writeStartElement("netemul");
    writeAttribute("version",QCoreApplication::applicationVersion() );
    foreach ( Device *i , myScene->devices() ) {
        writeEntity(i);
    }
    foreach ( Cable *j , myScene->connections() ) {
        writeStartElement("Cable");
        QPointF p1 = j->startPos();
        QPointF p2 = j->endPos();
        Device *dev1 = myScene->deviceInPoint( p1 );
        Device *dev2 = myScene->deviceInPoint( p2 );
        writeAttribute( "x1" , QString::number(p1.x()) );
        writeAttribute( "y1" , QString::number(p1.y()) );
        writeAttribute( "x2" , QString::number(p2.x()) );
        writeAttribute( "y2" , QString::number(p2.y()) );
        writeAttribute( "port1" , dev1->socketName(j) );
        writeAttribute( "port2" , dev2->socketName(j) );
        writeEndElement();
    }

    foreach ( TextItem *i , myScene->textItems() ) {        
        writeEntity(i);
    }
    writeEndDocument();
}
