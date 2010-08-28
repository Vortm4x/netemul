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

    QObject *instance = classFactory.createInstance(qName,parentObject);
    if ( instance ) {
        for ( int i = 0 ; i < atts.count() ; i++ ) {
            instance->setProperty( qPrintable(atts.localName(i)) , atts.value(i) );
        }

        qDebug() << qName << " created";

        bool isInvoked = false;
        const QMetaObject *obj = parentObject->metaObject();
        QString method = qPrintable(QString("add%1(%1*)").arg(qName));
        int m = obj->indexOfMethod( qPrintable(method) );
        if ( m != -1 ) {
            qDebug() << "find method! " << method ;
            QMetaMethod method = obj->method(m);
            method.invoke( parentObject , Qt::DirectConnection , Q_ARG(QObject*,instance) );
            isInvoked = true;
        }

        if ( !isInvoked ) {
            const QMetaObject *p = instance->metaObject();
            QString mStr = qName;
            do {
                method = qPrintable(QString("set%1(%1*)").arg(mStr));
                //            qDebug() << "class: " << p->className() << method;
                m = obj->indexOfMethod( QMetaObject::normalizedSignature(qPrintable(method)) );
                if ( m != -1 ) {
                    qDebug() << "find method! " << method ;
                    QMetaMethod method = obj->method(m);
                    method.invoke( parentObject , Qt::DirectConnection , Q_ARG(QObject*,instance) );
                    break;
                }
                p = p->superClass();
                mStr = p->className();
            } while ( p->superClass() );
        }
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

bool SceneXmlReader::characters(const QString &ch)
{
    return true;
}

bool SceneXmlReader::fatalError(const QXmlParseException &exception)
{
    return true;
}




