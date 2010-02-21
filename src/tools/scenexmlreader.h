#ifndef SCENEXMLREADER_H
#define SCENEXMLREADER_H

#include <QXmlStreamReader>

class MyCanvas;

class sceneXmlReader : public QXmlStreamReader
{
public:
    sceneXmlReader(MyCanvas *scene);
    void readScene(QIODevice *dev);
    void readUnknownElement();
private:
    MyCanvas *myScene;
    void readNetemul();
};

#endif // SCENEXMLREADER_H
