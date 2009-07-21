#ifndef BOXCHIP_H
#define BOXCHIP_H

#include "abstractchip.h"
#include <QVector>

class cableDev;
class devicePort;

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
    QString socketName(const cableDev *c) const;
private:
    const devicePort* socket(const QString &name) const;
protected:
    QVector<devicePort*> mySockets;
};

#endif // BOXCHIP_H
