#ifndef ROUTERDEVICE_H
#define ROUTERDEVICE_H

#include "smartdevice.h"

class routerDevice : public smartDevice
{
    Q_DECLARE_TR_FUNCTIONS(routerDevice)
public:
    enum { routerDev = 7 };
    int type() const { return routerDev; }
    routerDevice(int c = 4);
    void dialog();;
protected:
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
};



#endif // ROUTERDEVICE_H
