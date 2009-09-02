#ifndef RIPPROGRAMM_H
#define RIPPROGRAMM_H

#include "programmrep.h"

class smartDevice;
class routeModel;
class routeRecord;
class udpSocket;

static const qint8 infinity = 16;

/*!
  Реализует Rip-программу.
*/
class ripProgramm : public programmRep
{
    Q_OBJECT
public:
    enum { defaultTtl = 30 , RIP = 50 , ttl = 6 };
    ripProgramm();
    ~ripProgramm();
    void setDevice(smartDevice *s);
    bool interrupt(int u);
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
    void incTime();
private slots:
    void execute(QByteArray data);
private:
    void checkTable( routeRecord *r );
    void sendUpdate(bool isAll);
    quint16 mySocket;
    udpSocket *receiver;
    void addToTemp(routeRecord *r);
    void clearTemp();
    QList<routeRecord*> tempList;
    int timer;
    int interval;
    routeModel *model;
};
//----------------------------------------------

#endif // RIPPROGRAMM_H
