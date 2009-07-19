#ifndef BOXCHIP_H
#define BOXCHIP_H

#include "abstractchip.h"

class boxChip : public abstractChip
{
public:
    boxChip(int n = 4);
    QStringList sockets() const;
    bool setSocketsCount(int n);
    int socketsCount() const { return mySockets.size(); }
    devicePort* socket(const QString &name);
    bool isConnectSocket(const QString &str) { return socket(str)->isConnect(); }
    QString socketName(devicePort *p) const;
protected:
    QVector<devicePort*> mySockets;
};

#endif // BOXCHIP_H
