#ifndef MYCANVAS_H
#define MYCANVAS_H

#include <QObject>
#include "device.h"

class MyCanvas : public QObject
{
    Q_OBJECT
public:
    explicit MyCanvas(QObject *parent = 0);

    DeviceList devices() const { return myDevices; }
    Q_INVOKABLE void addDevice(Device *device) { myDevices << device; }

signals:

public slots:

private:
    DeviceList myDevices;

};

#endif // MYCANVAS_H
