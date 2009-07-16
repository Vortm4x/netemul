#ifndef BOXDEVICE_H
#define BOXDEVICE_H

#include "device.h"
#include "abstractchip.h"

class boxDevice : public deviceImpl , public abstractChip
{
    Q_DECLARE_TR_FUNCTIONS(boxDevice)
public:
    boxDevice();
    virtual ~boxDevice();
    void setManual(bool c) { myManual = c; }
    bool isManual() { return myManual; }
    bool setSocketCount(int n);
protected:
    bool myManual;
};

#endif // BOXDEVICE_H
