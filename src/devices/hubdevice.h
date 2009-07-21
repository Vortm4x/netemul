#ifndef HUBDEVICE_H
#define HUBDEVICE_H

#include "boxdevice.h"
#include "hubchip.h"

class frame;

class hubDevice : public boxDevice
{
public:
    int type() const { return hubDev; }
    enum { hubDev = 4 };
    hubDevice(int c = 4);
    ~hubDevice();
    void dialog();
protected:
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
};



#endif // HUBDEVICE_H
