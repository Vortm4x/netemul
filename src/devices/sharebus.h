#ifndef SHAREBUS_H
#define SHAREBUS_H

#include "device.h"
class QPainter;
class devicePort;

// Этот класс представляет собой общую шину, он тоже вынесен в отдельный класс
// который унаследован от device , так как у общей шины будет отличаться
// реализация и отображение от остальных устройств
//
class shareBus : public deviceImpl
{
public:
    enum { defaultWidth = 200 };
    shareBus(int c);
    devicePort* addInterface(QString str,int t);
    void dialog() { }
    bool hasTable() const { return false; }
private:
    int widthDev; // Это длина нашего устройства общей шины
    //и она может изменяться в отличии от других устройств
};

#endif // SHAREBUS_H
