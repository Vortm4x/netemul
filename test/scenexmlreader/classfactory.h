#ifndef CLASSFACTORY_H
#define CLASSFACTORY_H

#include <QObject>
#include <QString>
#include "device.h"
#include "computer.h"

class ClassFactory
{
public:
    ClassFactory();
    QObject* createInstance(const QString &name,QObject *parent) {
        if ( name == "Device" ) {
            return new Device(parent);
        } else if ( name == "Computer" ) {
            return new Computer(parent);
        } else if (name == "Interface" ) {
            return new Interface(parent);
        } else {
            return 0;
        }

    }
};

#endif // CLASSFACTORY_H
