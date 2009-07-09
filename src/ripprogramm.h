#ifndef RIPPROGRAMM_H
#define RIPPROGRAMM_H

#include "programm.h"

class smartDevice;
class routeRecord;
class ipPacket;

/*!
  Реализует Rip-программу.
*/
class ripProgramm : public programm
{
public:
    enum { defaultTtl = 30 , RIP = 50 , infinity = 16 };
    ripProgramm(smartDevice *d);
    void execute(ipPacket *p);
    void checkTable( routeRecord *r );
    void incTime();
    void sendRip();
    void interrupt(int u);
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
private:
    smartDevice *sd;
    int t;
    int inter;
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
