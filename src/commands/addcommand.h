#ifndef ADDCOMMAND_H
#define ADDCOMMAND_H

#include <QUndoCommand>
#include <QPointF>
class myCanvas;
class device;

class addCommand : public QUndoCommand
{
public:
    addCommand(myCanvas *s , QPointF point , int type);
    ~addCommand();
    void undo();
    void redo();
private:
    device *myDevice;
    myCanvas *scene;
    QPointF myPoint;
    int myType;
    bool isOnScene;
};

#endif // ADDCOMMAND_H
