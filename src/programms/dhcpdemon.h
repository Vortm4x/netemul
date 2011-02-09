#ifndef DHCPDEMON_H
#define DHCPDEMON_H

#include "ipaddress.h"
#include "macaddress.h"

class UdpSocket;
class DhcpPacket;
class Interface;
class StaticDhcpRecord;
class DhcpServerModel;

struct ClientState {
    int xid;
    int state;
    MacAddress mac;
    IpAddress ip;
    IpAddress mask;
    IpAddress gateway;
    int time;
    int requestTimer;
    enum { WAIT_REQUEST = 0, IN_USE = 1, DECLINE = 2 };
    ClientState(StaticDhcpRecord *rec);
    ClientState() { }
};

class DhcpDemon : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QString interfaceName READ interfaceName WRITE setInterfaceName )
    Q_PROPERTY( QString beginIp READ beginIp WRITE setBeginIp )
    Q_PROPERTY( QString endIp READ endIp WRITE setEndIp )
    Q_PROPERTY( QString mask READ mask WRITE setMask )
    Q_PROPERTY( QString gateway READ gateway WRITE setGateway )
    Q_PROPERTY( int time READ time WRITE setTime )
    Q_PROPERTY( int waitingTime READ waitingTime WRITE setWaitingTime )
    Q_PROPERTY( bool dynamic READ dynamic WRITE setDynamic )

public:    
    enum { CLIENT_SOCKET = 67 , SERVER_SOCKET = 68 };

    DhcpDemon(QObject* parent = 0);
    DhcpDemon(Interface *inter);
    ~DhcpDemon();
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
    DhcpServerModel* dhcpModel() { return myDhcpModel; }
    void incTime();
    IpAddress giveDynamicIp() const;

    void read(QDataStream &stream);

// Обработка пакетов
public:
    void executeDiscover(DhcpPacket packet);
    void executeRequest(DhcpPacket packet);
    void executeDecline(DhcpPacket packet);

// Функция инициализации для конструкторов
private:
    void initialize();

// Функции создания и отправки ответа
private:
    void makeAnswer(ClientState* client, int type);
    void sendDhcp(DhcpPacket packet) const;
    DhcpPacket createDhcpPacket(ClientState *client, int state) const;

// Функции выбора и нахождения записи клиента
private:
    ClientState* chooseStatic(DhcpPacket packet);
    ClientState* chooseDynamic(DhcpPacket packet);
    ClientState* findClient( int xid ) const;
    ClientState* findClient(IpAddress ip) const;

private:
    QList<ClientState*> clients;
    QString myInterface;
    DhcpServerModel *myDhcpModel;
    IpAddress myBeginIp;
    IpAddress myEndIp;
    IpAddress myMask;
    IpAddress myGateway;
    int myTime;
    int myWaitingTime;
    bool myDynamic;
};

#endif // DHCPDEMON_H
