#include "scenexmlreader.h"
#include "mycanvas.h"
#include "device.h"
#include "classfactory.h"

#include <QtDebug>
#include <QMetaMethod>

static ClassFactory classFactory;

SceneXmlReader::SceneXmlReader(MyCanvas *scene) : myScene(scene) , parentObject(myScene) ,
                                                    isCorrect(false)
{
}

bool SceneXmlReader::startElement(const QString&, const QString&,
                                  const QString &qName, const QXmlAttributes &atts)
{
    if ( !isCorrect && qName != "netemul" ) {
        errString = QObject::tr("The file is not an netemul file.");
        return false;
    }

    if ( qName == "netemul" ) {
        isCorrect = true;
        return true;
    }

    if ( qName == "Cable" ) {
        Device *d1 = myScene->deviceInPoint( QPointF(atts.value("x1").toDouble() , atts.value("y1").toDouble()) );
        Device *d2 = myScene->deviceInPoint( QPointF(atts.value("x2").toDouble() , atts.value("y2").toDouble()) );
        QString port1 = atts.value("port1");
        QString port2 = atts.value("port2");
        myScene->createConnection( d1, d2 , port1 , port2 );
        return true;
    }

    QObject *instance = classFactory.createInstance(qName,parentObject);
    if ( instance ) {
        for ( int i = 0 ; i < atts.count() ; i++ ) {
            instance->setProperty( qPrintable(atts.localName(i)) , atts.value(i) );
        }

        qDebug() << qName << " created";


        const QMetaObject *obj = parentObject->metaObject();

        const QMetaObject *p = instance->metaObject();
        QString mStr = qName;
        do {
            bool isInvoked = false;
            QString method = qPrintable(QString("add%1(%1*)").arg(mStr));
            int m = obj->indexOfMethod( qPrintable(method) );
//            qDebug() << "class: " << p->className() << method;
            if ( m != -1 ) {
                qDebug() << "find method! " << method ;
                QMetaMethod method = obj->method(m);
                method.invoke( parentObject , Qt::DirectConnection , Q_ARG(QObject*,instance) );
                isInvoked = true;
                break;
            }

            if ( !isInvoked ) {
                method = qPrintable(QString("set%1(%1*)").arg(mStr));
                //                            qDebug() << "class: " << p->className() << method;
                m = obj->indexOfMethod( QMetaObject::normalizedSignature(qPrintable(method)) );
                if ( m != -1 ) {
                    qDebug() << "find method! " << method ;
                    QMetaMethod method = obj->method(m);
                    method.invoke( parentObject , Qt::DirectConnection , Q_ARG(QObject*,instance) );
                    break;
                }
            }
            p = p->superClass();
            mStr = p->className();
        } while ( p->superClass() );

        parentObject = instance;
        currentElements.push(qName);
        //        qDebug() << "qu " << qName << parentObject;
    }
    return true;
}

bool SceneXmlReader::endElement(const QString&, const QString&, const QString &qName)
{
    if ( !currentElements.isEmpty() && currentElements.last() == qName ) {
//        qDebug() << currentElements;
        currentElements.pop();
        parentObject = parentObject->parent();
        if ( !parentObject ) {
            parentObject = myScene;
        }
//        qDebug() << "after " << qName << parentObject;
    }
    return true;
}

bool SceneXmlReader::characters(const QString&)
{
    return true;
}

bool SceneXmlReader::fatalError(const QXmlParseException&)
{
    return true;
}




