#ifndef DHCPCLIENTPROGRAMM_H
#define DHCPCLIENTPROGRAMM_H

#include "programmrep.h"

static const int CLIENT_SOCKET = 67;
static const int SERVER_SOCKET = 68;
static const int FIVE_MINUTE = 300;

class dhcpClientProgramm : public programmRep
{
    Q_OBJECT
public:
    enum { DHCPClient = 51 };
    dhcpClientProgramm();
    ~dhcpClientProgramm() { }
    void setEnable(bool b);
    bool interrupt(int) { return false; }
    void incTime();
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
private:
    void sendDiscover();
    bool hasSetting;
    int time;
};

#endif // DHCPCLIENTPROGRAMM_H
