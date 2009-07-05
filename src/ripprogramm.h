#ifndef RIPPROGRAMM_H
#define RIPPROGRAMM_H

#include "programm.h"

class smartDevice;
class routeRecord;
class ipPacket;

class ripProgramm : public programm
{
public:
    enum { defaultTtl = 30 , RIP = 50 };
    ripProgramm(smartDevice *d);
    void execute(ipPacket *p);
    void checkTable( routeRecord *r );
    void incTime();
    void sendRip();
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
private:
    smartDevice *sd;
    int t;
    int inter;
};

#endif // RIPPROGRAMM_H
