#ifndef DEVICE_H
#define DEVICE_H

#include <QString>

class DevicePort;

class Device
{
public:
    Device();
    DevicePort* findPortByName(const QString &str) {
        return 0;
    }
};

#endif // DEVICE_H
