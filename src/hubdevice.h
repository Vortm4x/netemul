#ifndef HUBDEVICE_H
#define HUBDEVICE_H

#include "boxdevice.h"
#include "deviceport.h"

class hubChip;
class frame;

class hubDevice : public boxDevice
{
public:
    enum { hubDev = 4 };
    hubDevice(int c);
    ~hubDevice();
    devicePort* addInterface(QString str,int t);
    void dialog();
protected:
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
private:
    hubChip *chip;
};


#endif // HUBDEVICE_H
