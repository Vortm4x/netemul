#ifndef ADDCABLECOMMAND_H
#define ADDCABLECOMMAND_H

#include <QUndoCommand>

class myCanvas;
class cableDev;

class addCableCommand : public QUndoCommand
{
public:
    addCableCommand(myCanvas *s, cableDev *c);
    ~addCableCommand();
    void redo();
    void undo();
private:
    myCanvas *scene;
    cableDev *cable;
};

#endif // ADDCABLECOMMAND_H
