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

    const QString& note() const { return myNote; }
    void setNote(const QString &n) { myNote = n; }

    qint8 type() { return myType; }

    void sendEvent();
    virtual bool hasTable() const { return false; }
protected:
    QList<devicePort*> mySockets;
private:
    qint8 myType;
    QString myNote;
};

#endif // DEVICEIMPL_H
