#ifndef CLASSFACTORY_H
#define CLASSFACTORY_H

#include <QString>
#include <QObject>

class ClassFactory
{
public:
    ClassFactory();

public:
    QObject* createInstance(const QString &str,QObject *parent);
};

#endif // CLASSFACTORY_H
