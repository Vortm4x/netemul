#ifndef DEVICEIMPL_H
#define DEVICEIMPL_H

#include "deviceport.h"

class deviceImpl
{
public:
    deviceImpl();
    virtual ~deviceImpl() { }
    QList<devicePort*> sockets() const { return mySockets; }
    void setCheckedSocket(const QString &s);
    devicePort* socket(const QString &name);
    virtual devicePort* addSocket(const QString &str) = 0;
    virtual void removeSocket(devicePort* inter);
protected:
    QList<devicePort*> mySockets;
};

#endif // DEVICEIMPL_H
