#ifndef BOXCHIP_H
#define BOXCHIP_H

#include "abstractchip.h"
#include <QVector>

class cableDev;
class devicePort;

class boxChip : public abstractChip
{
    Q_OBJECT
public:
    boxChip(int n = 4);
    virtual ~boxChip();
    QStringList sockets() const;
    bool setSocketsCount(int n);
    int socketsCount() const { return mySockets.size(); }
    bool isConnectSocket(const QString &str) const;
    void addConnection(const QString &port, cableDev *c);
    void deleteConnection(cableDev *c);
    QString socketName(const cableDev *c) const;
    virtual void deciSecondTimerEvent();
    virtual void secondTimerEvent() { }
    devicePort* socket(const QString &name);
    bool isBusy() const;
    virtual void addSocket(int n);
    virtual bool isSharedBusy(cableDev*) const { return false; }
    virtual void detectCollision() { }
#ifndef __TESTING__
    virtual void write(QDataStream &stream) const;
    virtual void read(QDataStream &stream);
#endif  
protected:
    QVector<devicePort*> mySockets;
};

#endif // BOXCHIP_H
