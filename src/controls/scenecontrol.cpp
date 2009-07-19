#include "scenecontrol.h"
#include "device.h"


sceneControl::sceneControl(QObject *parent , myCanvas *s ) : QObject(parent) , scene(s)
{
    connect( scene , SIGNAL(selectionChanged()) , SLOT(observeSelection()) );
}

void sceneControl::observeSelection()
{
    if ( device *t = scene->oneSelectedDevice() ) {
        emit selectTableDevice( t->hasTable() );
        emit selectOneDevice(true);
        emit selectSmartDevice( t->isSmart() );
    }
    else {
        emit selectSmartDevice(false);
        emit selectOneDevice(false);
        emit selectTableDevice(false);
    }
}

QString sceneControl::tableName() const
{
    return scene->oneSelectedDevice()->tableName();
}

void sceneControl::propertyDialog() const
{
    device *t = scene->oneSelectedDevice();
    t->dialog();
}

void sceneControl::tableDialog() const
{
    device *t = scene->oneSelectedDevice();
    t->tableDialog();
}

void sceneControl::adapterDialog() const
{
    device *t = scene->oneSelectedDevice();
    t->adapterDialog();
}

void sceneControl::programmsDialog() const
{
    device *t = scene->oneSelectedDevice();
    t->programmsDialog();
}
