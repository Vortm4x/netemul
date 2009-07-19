#ifndef SCENECONTROL_H
#define SCENECONTROL_H

#include "mycanvas.h"

class sceneControl : public QObject
{
    Q_OBJECT
public:
    sceneControl(QObject *parent,myCanvas *s);
    bool isSelect() const { if ( scene->oneSelectedDevice() ) return true; return false; }
    QString tableName() const;
private:
    myCanvas *scene;
public slots:
    void observeSelection();
    void propertyDialog() const;
    void tableDialog() const;
    void adapterDialog() const;
    void programmsDialog() const;
signals:
    void selectOneDevice(bool);
    void selectTableDevice(bool);
    void selectSmartDevice(bool);
};

#endif // SCENECONTROL_H
