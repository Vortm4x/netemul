#ifndef HUBDEVICE_H
#define HUBDEVICE_H

#include "boxdevice.h"

class frame;

class hubDevice : public boxDevice
{
    Q_OBJECT
public:
    int type() const { return hubDev; }
    enum { hubDev = 4 };
    hubDevice(int c = 0);
    ~hubDevice();
    void dialog();
    bool isSharedBusy(cableDev *c) const;
protected:
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
};



#endif // HUBDEVICE_H
