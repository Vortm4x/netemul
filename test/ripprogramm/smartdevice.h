#ifndef SMARTDEVICE_H
#define SMARTDEVICE_H

#include "ipaddress.h"
#include "ippacket.h"

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
private:
    routeModel *table;
};

#endif // SMARTDEVICE_H
