#ifndef SCENEXMLREADER_H
#define SCENEXMLREADER_H

#include <QXmlStreamReader>

class myCanvas;

class sceneXmlReader : public QXmlStreamReader
{
public:
    sceneXmlReader(myCanvas *scene);
    void readScene(QIODevice *dev);
    void readUnknownElement();
private:
    myCanvas *myScene;
    void readNetemul();
};

#endif // SCENEXMLREADER_H
