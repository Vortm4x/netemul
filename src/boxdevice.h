#ifndef BOXDEVICE_H
#define BOXDEVICE_H

#include "device.h"
#include "boxchip.h"
#include "abstractchip.h"


class boxDevice : public deviceImpl
{
public:
    boxDevice();
    virtual ~boxDevice();
    devicePort* socket(const QString &name) { return chip->socket(name); }
    bool isConnectSocket(const QString &socket) const { return chip->isConnectSocket(socket); }
    QStringList sockets() const { return chip->sockets(); }
    QString socketName(devicePort *p) const { return chip->socketName(p); }
    void setManual(bool c) { myManual = c; }
    bool isManual() { return myManual; }
protected:
    bool myManual;
    boxChip *chip;
};

#endif // BOXDEVICE_H
