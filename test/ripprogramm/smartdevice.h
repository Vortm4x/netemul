#ifndef SMARTDEVICE_H
#define SMARTDEVICE_H

#include "ipaddress.h"
#include "ippacket.h"
#include "interface.h"
#include <QVector>

class routeModel;

class smartDevice
{
public:
    enum { addNet = 100 , delNet = 101 };
    smartDevice();
    ~smartDevice();
    bool isRouter() const { return true; }
    ipAddress findInterfaceIp(ipAddress) { return ipAddress(); }
    routeModel* routeTable() { return table; }
    QVector<interface*> interfaces() { return myInterfaces; }
private:
    routeModel *table;
    QVector<interface*> myInterfaces;
};

#endif // SMARTDEVICE_H
