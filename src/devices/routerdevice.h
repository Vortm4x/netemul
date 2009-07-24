#ifndef ROUTERDEVICE_H
#define ROUTERDEVICE_H

#include "smartdevice.h"

class routerDevice : public smartDevice
{
public:
    enum { routerDev = 7 };
    int type() const { return routerDev; }
    routerDevice(int c = 4);
    void dialog();
protected:
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
public slots:
    void setSocketsCount(int n);
};



#endif // ROUTERDEVICE_H
