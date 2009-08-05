#include "smartdevice.h"
#include "routemodel.h"

smartDevice::smartDevice()
{
    table = new routeModel;
}

smartDevice::~smartDevice()
{
    delete table;
}
