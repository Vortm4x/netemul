#ifndef DHCPSERVERPROGRAMM_H
#define DHCPSERVERPROGRAMM_H

#include "programmrep.h"

class udpSocket;

class dhcpServerProgramm : public programmRep
{
    Q_OBJECT
public:
    enum { DHCPServer = 52 , CLIENT_SOCKET = 67 , SERVER_SOCKET = 68 };
    dhcpServerProgramm();
    ~dhcpServerProgramm() { }
    void setDevice(smartDevice *s);
    void incTime() { }
    bool interrupt(int) { return false; }
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
public slots:
    void execute(QByteArray data);
private:
    udpSocket *receiver;
};

#endif // DHCPSERVERPROGRAMM_H
