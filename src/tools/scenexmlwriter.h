#ifndef SCENEXMLWRITER_H
#define SCENEXMLWRITER_H

#include <QXmlStreamWriter>

class MyCanvas;

class sceneXmlWriter : public QXmlStreamWriter
{
public:
    sceneXmlWriter(MyCanvas *scene);
    void writeScene(QIODevice *dev);
private:
    MyCanvas *myScene;
};

#endif // SCENEXMLWRITER_H
