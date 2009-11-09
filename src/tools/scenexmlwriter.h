#ifndef SCENEXMLWRITER_H
#define SCENEXMLWRITER_H

#include <QXmlStreamWriter>

class myCanvas;

class sceneXmlWriter : public QXmlStreamWriter
{
public:
    sceneXmlWriter(myCanvas *scene);
    void writeScene(QIODevice *dev);
private:
    myCanvas *myScene;
};

#endif // SCENEXMLWRITER_H
