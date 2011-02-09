#include "dhcpdemon.h"
#include "interface.h"
#include "udppacket.h"
#include "dhcppacket.h"
#include "dhcpservermodel.h"

dhcpDemon::dhcpDemon(Interface *inter) {
    myInterface = inter->name();
    myDhcpModel = new DhcpServerModel;
    myDynamic = false;
    myTime = 300;
    myWaitingTime = 60;
}

dhcpDemon::~dhcpDemon() {
    delete myDhcpModel;
    //delete receiver;
    clients.clear();
}

void dhcpDemon::executeDiscover(dhcpPacket packet)
{
    clientState *client = findClient( packet.xid() );
    if ( client && client->state == clientState::IN_USE ) return;
    client = chooseStatic(packet);
    if ( client ) {
        makeAnswer(client, dhcpPacket::DHCPOFFER);
        return;
    }
    if ( !myDynamic ) return;
    client = chooseDynamic(packet);
    if ( client ) makeAnswer(client,dhcpPacket::DHCPOFFER);
}

void dhcpDemon::executeRequest(dhcpPacket packet)
{
    clientState *client = findClient( packet.xid() );
    if ( !client ) return;
//    if ( packet.siaddr() != myDevice->adapter(myInterface)->ip() ) {
//        clients.removeOne(client);
//        delete client;
//        return;
//    }
    makeAnswer( client, dhcpPacket::DHCPACK );
}

void dhcpDemon::executeDecline(dhcpPacket packet)
{
    clientState *client = findClient(packet.yiaddr());
    if ( !client ) return;
    client->state = clientState::DECLINE;
}

clientState* dhcpDemon::chooseStatic(dhcpPacket packet)
{
    StaticDhcpRecord *rec = myDhcpModel->recordWithMac(packet.chaddr());
    if ( !rec ) return 0;
    clientState *client = new clientState(rec);
    if ( findClient(client->ip) ) return 0;
    client->requestTimer = 0;
    client->xid = packet.xid();
    clients << client;
    return client;
}

/*!
  Выбираем адрес из динамического диапазона.
  @return указатель на созданную запись.
  */
clientState* dhcpDemon::chooseDynamic(dhcpPacket packet)
{
    clientState *cl = new clientState;
    cl->requestTimer = 0;
    clientState *c = findClient(packet.yiaddr());
    if ( !packet.yiaddr().isEmpty() && !c ) cl->ip = packet.yiaddr();
    else cl->ip = giveDynamicIp();
    if ( cl->ip.isEmpty() ) return NULL;
    cl->mac = packet.chaddr();
    cl->xid = packet.xid();
    cl->time = myTime;
    cl->mask = myMask;
    cl->gateway = myGateway;
    clients << cl;
    return cl;
}

/*!
  Создаем dhcp пакет.
  @param client - запись клиента на основе которой будем создавать пакет.
  @param state - тип отправляемого сообщения.
  @return созданный пакет.
  */
dhcpPacket dhcpDemon::createDhcpPacket( clientState *client, int state ) const
{
    if ( state == dhcpPacket::DHCPOFFER ) client->state = clientState::WAIT_REQUEST;
    else client->state = clientState::IN_USE;
    dhcpPacket p;
    p.setType( state );
    p.setXid( client->xid );
    p.setChaddr( client->mac );
    p.setYiaddr( client->ip );
    p.setMask( client->mask );
    p.setGateway( client->gateway );
    //p.setSiaddr( myDevice->adapter(myInterface)->ip() );
    p.setTime( client->time );
    return p;
}
//------------------------------------------------------------

void dhcpDemon::sendDhcp(dhcpPacket packet) const
{
    udpPacket udp;
    udp.setSender( SERVER_SOCKET );
    udp.setReceiver( CLIENT_SOCKET );
    udp.pack( packet.toData() );
    //ipPacket p( myDevice->adapter(myInterface)->ip(), ipAddress::full() );
    //p.pack( udp.toData() );
    //p.setUpProtocol( ipPacket::udp );
    //myDevice->adapter(myInterface)->sendPacket(p);
}

void dhcpDemon::makeAnswer(clientState *client, int type)
{
    dhcpPacket dhcp = createDhcpPacket(client,type);
    sendDhcp(dhcp);
}

/*! Ищет в списке клиента с данным идентификатрором и проверкой состояния записи.
  @param xid - идентификатрор.
  @return указатель на запись из списка, если xid совпали, или 0 в противном случае.
  */
clientState* dhcpDemon::findClient(int xid) const
{
    foreach ( clientState *i, clients )
        if ( i->xid == xid && i->state != clientState::DECLINE ) return i;
    return 0;
}
//------------------------------------------------------------
/*! Ищет в списке клиента с данным ip-адресом.
  @param ip - адрес.
  @return указатель на запись из списка, если ip совпали, или 0 в противном случае.
  */
clientState* dhcpDemon::findClient(IpAddress ip) const
{
    foreach ( clientState *i, clients )
        if ( i->ip == ip ) return i;
    return 0;
}
//--------------------------------------------------------------
/*!
  Выбирает ip-адрес из динамического диапазона
  @return выбранный адрес, или "0.0.0.0", если нет свободных адресов.
  */
IpAddress dhcpDemon::giveDynamicIp() const
{
    bool isContains = false;
    quint32 i = myBeginIp.toInt();
    while ( i <= myEndIp.toInt() ) {
//        isContains = myDhcpModel->containRecord( ipAddress(i) ) || findClient(ipAddress(i))
//                     || myDevice->adapter(myInterface)->ip().toInt() == i;
        if ( isContains ) {
            i++;
            isContains = false;
        }
        else {
            return IpAddress(i);
        }
    }
    return IpAddress("0.0.0.0");
}
//-------------------------------------------------------------
void dhcpDemon::incTime() {
    bool canDelete = false;
    foreach ( clientState *i, clients ) {
        if ( i->state == clientState::WAIT_REQUEST ) {
            if ( ++i->requestTimer == myWaitingTime ) canDelete = true;
        }
        else if ( i->state == clientState::IN_USE && --i->time == 0 ) canDelete = true;
        if ( canDelete ) {
            canDelete = false;
            clients.removeOne(i);
            delete i;
        }
    }
}

void dhcpDemon::read(QDataStream &stream)
{
    stream >> myInterface;
    stream >> myBeginIp;
    stream >> myEndIp;
    stream >> myMask;
    stream >> myGateway;
    stream >> myTime;
    stream >> myDynamic;
    stream >> myWaitingTime;
}
