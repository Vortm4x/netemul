#ifndef SCENEXMLREADER_H
#define SCENEXMLREADER_H

#include <QXmlDefaultHandler>
#include <QStack>

class MyCanvas;

class SceneXmlReader : public QXmlDefaultHandler
{
public:
    SceneXmlReader(MyCanvas *scene);

public:
    bool startElement(const QString&, const QString&, const QString &qName, const QXmlAttributes &atts);
    bool endElement(const QString&, const QString&, const QString &qName);
    bool characters(const QString&);
    bool fatalError(const QXmlParseException&);
    QString errorString() {
        return errString;
    }
private:
    MyCanvas *myScene;
    QString errString;
    QObject *parentObject;
    bool isCorrect;
    QStack<QString> currentElements;
};

#endif // SCENEXMLREADER_H
