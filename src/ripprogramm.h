#ifndef RIPPROGRAMM_H
#define RIPPROGRAMM_H

#include "programm.h"
#include "ipaddress.h"

class smartDevice;
class routeRecord;
class ipPacket;

class ripProgramm : public programm
{
public:
    ripProgramm(smartDevice *d);
    void execute(ipPacket *p);
    void checkTable( routeRecord *r );
    void incTime();
    void sendRip();
private:
    smartDevice *sd;
    int t;
    int inter;
};

#endif // RIPPROGRAMM_H
