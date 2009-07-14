#ifndef RIPPROGRAMM_H
#define RIPPROGRAMM_H

#include "programm.h"

class smartDevice;
class routeRecord;
class ipPacket;

static const qint8 infinity = 16;

/*!
  Реализует Rip-программу.
*/
class ripProgramm : public programm
{
public:
    enum { defaultTtl = 30 , RIP = 50 };
    ripProgramm(smartDevice *d);
    void execute(ipPacket *p);
    void checkTable( routeRecord *r );
    void incTime();
    void sendUpdate(bool isAll);
    bool interrupt(int u);
    routeRecord* findChanged() const;
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
private:
    smartDevice *sd;
    int timer;
    int interval;
};
//----------------------------------------------
/*!
  Записывает отличительные черты RIP в поток.
  @param stream - поток для записи.
*/
inline void ripProgramm::write(QDataStream &stream) const
{
    stream << RIP;
    programm::write(stream); // и вызываем функцию предка.
}
//---------------------------------------------------
/*!
  Считывает отличительные черты RIP из потока.
  @param stream - поток для чтения.
*/
inline void ripProgramm::read(QDataStream &stream)
{
    programm::read(stream);
}
//---------------------------------------------------
#endif // RIPPROGRAMM_H
