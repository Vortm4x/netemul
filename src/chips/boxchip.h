#ifndef BOXCHIP_H
#define BOXCHIP_H

#include "abstractchip.h"
#include <QVector>

class cableDev;
class devicePort;
class frame;

class boxChip : public abstractChip
{
public:
    boxChip(int n = 4);
    virtual ~boxChip() { }
    QStringList sockets() const;
    bool setSocketsCount(int n);
    int socketsCount() const { return mySockets.size(); }
    bool isConnectSocket(const QString &str) const;
    void addConnection(const QString &port, cableDev *c);
    void deleteConnection(cableDev *c);
    QString socketName(const cableDev *c) const;
    virtual void receiveEvent(frame *fr,devicePort *sender) = 0;
    void deciSecondTimerEvent();
    devicePort* socket(const QString &name);
    virtual void write(QDataStream &stream) const;
    virtual void read(QDataStream &stream);
protected:
    QVector<devicePort*> mySockets;
};

#endif // BOXCHIP_H
