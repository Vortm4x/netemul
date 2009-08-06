#ifndef RIPPROGRAMM_H
#define RIPPROGRAMM_H

#include "programmrep.h"

class smartDevice;
class routeModel;
class routeRecord;
class ipPacket;

static const qint8 infinity = 16;

/*!
  Реализует Rip-программу.
*/
class ripProgramm : public programmRep
{
public:
    enum { defaultTtl = 30 , RIP = 50 , ttl = 6 };
    ripProgramm();
    ~ripProgramm();
    void setDevice(smartDevice *s);
    void execute(ipPacket &p);
    void checkTable( routeRecord *r );
    void incTime();
    void sendUpdate(bool isAll);
    bool interrupt(int u);
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
private:
    void addToTemp(routeRecord *r);
    void clearTemp();
    QList<routeRecord*> tempList;
    smartDevice *sd;
    int timer;
    int interval;
    routeModel *model;
};
//----------------------------------------------

#endif // RIPPROGRAMM_H
