#ifndef SCENEXMLWRITER_H
#define SCENEXMLWRITER_H

#include <QXmlStreamWriter>

class MyCanvas;

class SceneXmlWriter : public QXmlStreamWriter
{
public:
    SceneXmlWriter(MyCanvas *scene);
    void writeScene(QIODevice *dev);

private:
    void writeEntity(const QObject *entity);

private:    
    MyCanvas *myScene;
};

#endif // SCENEXMLWRITER_H
