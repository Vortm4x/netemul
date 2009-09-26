#ifndef DELETECOMMAND_H
#define DELETECOMMAND_H

#include <QUndoCommand>

class QGraphicsItem;
class myCanvas;
class cableDev;
class device;

class deleteCommand : public QUndoCommand
{
public:
    deleteCommand(myCanvas *s, QList<QGraphicsItem*> list);
    ~deleteCommand();
    void undo();
    void redo();
    void deleteConnection(cableDev *cable);
    void createConnection(cableDev *cable);
private:
    myCanvas *scene;
    QList<cableDev*> extractCables;
    QList<device*> extractDevices;
    bool isOnScene;
};

#endif // DELETECOMMAND_H
