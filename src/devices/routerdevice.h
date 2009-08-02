#ifndef ROUTERDEVICE_H
#define ROUTERDEVICE_H

#include "smartdevice.h"

class routerDevice : public smartDevice
{
public:
    enum { routerDev = 7 };
    int type() const { return routerDev; }
    routerDevice(int c = 0);
    bool canManageInterface() const { return false; }
    void dialog();
protected:
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
public slots:
    void setSocketsCount(int n);
};

class routerSetting : public smartSetting
{
public:
    routerSetting(routerDevice *r) : smartSetting(r) , rd(r) { }
    void setSocketsCount(int n) { rd->setSocketsCount(n); }
private:
    routerDevice *rd;
};


#endif // ROUTERDEVICE_H
