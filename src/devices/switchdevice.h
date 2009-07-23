#ifndef SWITCHDEVICE_H
#define SWITCHDEVICE_H

#include "boxdevice.h"
#include "macaddress.h"

class frame;



// Пока такое коротенькое объявление для свитча
// Все его комментарии действительны для других устройств
class switchDevice : public boxDevice
{
public:
    enum { switchDev = 5 };
    int type() const { return switchDev; }
    switchDevice(int c = 4); //Пока конструктор и прорисовка
    ~switchDevice();
    void updateMac(int u);
    void dialog();
    void tableDialog();
    bool hasTable() const { return true; }
protected:
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
};


#endif // SWITCHDEVICE_H
