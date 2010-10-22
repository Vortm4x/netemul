#ifndef SNIFFERPROGRAMM_H
#define SNIFFERPROGRAMM_H

#include "programmrep.h"
#include "ipaddress.h"
#include "macaddress.h"
#include "frame.h"

class ArpRecord;

class SpoofingProgram : public Program
{
    Q_OBJECT
public:
    enum { SPOOFING = 3 };
    SpoofingProgram(QObject *parent = 0);
    ~SpoofingProgram() { }
    bool interrupt(int) { return false; }
    void incTime();
    void showProperty();
    void setDevice(SmartDevice *s);
    QString featureName() const { return "s"; }
    int id() const { return SPOOFING; }
    void setServerIp(IpAddress server) { myServerIp = server; }
    IpAddress serverIp() const { return myServerIp; }
    void setClientIp(IpAddress server) { myClientIp = server; }
    IpAddress clientIp() const { return myClientIp; }
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
public slots:
    void execute(ArpRecord *record);
private:
    void sendOneAnswer( IpAddress sender , IpAddress receiver , macAddress receiverMac );
    void sendAnswers();

    IpAddress myServerIp;
    IpAddress myClientIp;
    bool isReady;
    bool hasAttack;
    macAddress myServerMac;
    macAddress myClientMac;
};

#endif // SNIFFERPROGRAMM_H
