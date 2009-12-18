#ifndef SNIFFERPROGRAMM_H
#define SNIFFERPROGRAMM_H

#include "programmrep.h"
#include "ipaddress.h"
#include "macaddress.h"
#include "frame.h"

class arpRecord;

class spoofingProgramm : public programmRep
{
    Q_OBJECT
public:
    enum { SPOOFING = 3 };
    spoofingProgramm();
    ~spoofingProgramm() { }
    bool interrupt(int) { return false; }
    void incTime();
    void showProperty();
    void setDevice(smartDevice *s);
    QString featureName() const { return "S"; }
    int id() const { return SPOOFING; }
    void setServerIp(ipAddress server) { myServerIp = server; }
    ipAddress serverIp() const { return myServerIp; }
    void setClientIp(ipAddress server) { myClientIp = server; }
    ipAddress clientIp() const { return myClientIp; }
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
public slots:
    void execute(arpRecord *record);
private:
    void sendOneAnswer( ipAddress sender , ipAddress receiver , macAddress receiverMac );
    void sendAnswers();

    ipAddress myServerIp;
    ipAddress myClientIp;
    bool isReady;
    bool hasAttack;
    macAddress myServerMac;
    macAddress myClientMac;
};

#endif // SNIFFERPROGRAMM_H
