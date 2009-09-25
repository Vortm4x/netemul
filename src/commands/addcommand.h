#ifndef ADDCOMMAND_H
#define ADDCOMMAND_H

#include <QUndoCommand>
#include <QPointF>
class myCanvas;

class addCommand : public QUndoCommand
{
public:
    addCommand(myCanvas *s , QPointF point , int type);
    void undo();
    void redo();
private:
    myCanvas *scene;
    QPointF myPoint;
    int myType;
};

#endif // ADDCOMMAND_H
