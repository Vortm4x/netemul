#ifndef DHCPCLIENTPROGRAMM_H
#define DHCPCLIENTPROGRAMM_H

#include "programmrep.h"

static const int FIVE_MINUTE = 300;

class dhcpClientProgramm : public programmRep
{
    Q_OBJECT
public:
    enum { DHCPClient = 51 ,CLIENT_SOCKET = 67, SERVER_SOCKET = 68  };
    enum state { NONE = 0 , WAIT_VARIANT = 1 , WAIT_RESPONCE = 2 , ALL_RIGHT = 3 };
    dhcpClientProgramm();
    ~dhcpClientProgramm() { }
    void setEnable(bool b);
    bool interrupt(int) { return false; }
    void incTime();
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
private:
    int xid;
    state myState;
    void sendDiscover();
    int time;
};

#endif // DHCPCLIENTPROGRAMM_H
