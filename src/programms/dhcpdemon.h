#ifndef DHCPDEMON_H
#define DHCPDEMON_H

#include "ipaddress.h"
#include "macaddress.h"

class UdpSocket;
class dhcpPacket;
class Interface;
class staticDhcpRecord;
class dhcpServerModel;

struct clientState {
    int xid;
    int state;
    macAddress mac;
    IpAddress ip;
    IpAddress mask;
    IpAddress gateway;
    int time;
    int requestTimer;
    enum { WAIT_REQUEST = 0, IN_USE = 1, DECLINE = 2 };
    clientState(staticDhcpRecord *rec);
    clientState() { }
};

class dhcpDemon
{
public:    
    enum { CLIENT_SOCKET = 67 , SERVER_SOCKET = 68 };

    dhcpDemon(Interface *inter);
    ~dhcpDemon();
    void setInterfaceName( QString inter ) { myInterface = inter; }
    void setBeginIp(QString ip) { myBeginIp.setIp(ip); }
    void setEndIp(QString ip) { myEndIp.setIp(ip); }
    void setMask(QString m) { myMask.setIp(m); }
    void setGateway(QString g) { myGateway.setIp(g); }
    void setTime(int t) { myTime = t; }
    void setWaitingTime(int t) { myWaitingTime = t; }
    void setDynamic(bool b) { myDynamic = b; }
    QString interfaceName() const { return myInterface; }
    QString beginIp() const { return myBeginIp.toString(); }
    QString endIp() const { return myEndIp.toString(); }
    QString mask() const { return myMask.toString(); }
    QString gateway() const { return myGateway.toString(); }
    int time() const { return myTime; }
    int waitingTime() const { return myWaitingTime; }
    bool dynamic() const { return myDynamic; }
    dhcpServerModel* dhcpModel() { return myDhcpModel; }
    void incTime();
    IpAddress giveDynamicIp() const;

    void read(QDataStream &stream);

// Обработка пакетов
public:
    void executeDiscover(dhcpPacket packet);
    void executeRequest(dhcpPacket packet);
    void executeDecline(dhcpPacket packet);

// Функции создания и отправки ответа
private:
    void makeAnswer(clientState* client, int type);
    void sendDhcp(dhcpPacket packet) const;
    dhcpPacket createDhcpPacket( clientState *client, int state ) const;

// Функции выбора и нахождения записи клиента
private:
    clientState* chooseStatic(dhcpPacket packet);
    clientState* chooseDynamic(dhcpPacket packet);
    clientState* findClient( int xid ) const;
    clientState* findClient(IpAddress ip) const;

private:
    QList<clientState*> clients;
    QString myInterface;
    dhcpServerModel *myDhcpModel;
    IpAddress myBeginIp;
    IpAddress myEndIp;
    IpAddress myMask;
    IpAddress myGateway;
    int myTime;
    int myWaitingTime;
    bool myDynamic;
};

#endif // DHCPDEMON_H
