#include "dhcpclientprogramm.h"
#include "smartdevice.h"
#include "udpsocket.h"
#include "dhcppacket.h"

dhcpClientProgramm::dhcpClientProgramm()
{
    myName = tr("DHCP client");
    myState = NONE;
    time = 0;
}

void dhcpClientProgramm::setEnable(bool b)
{
    programmRep::setEnable(b);
    if ( myState != NONE && !b ) return; // Не нужно спрашивать настройки при выключении или если они уже есть.
    sendDiscover();
}

void dhcpClientProgramm::incTime()
{
    time++;
    if (  myState == NONE && time%FIVE_MINUTE == 0 ) sendDiscover();
}

void dhcpClientProgramm::sendDiscover()
{
    xid = qrand()%5000;
    dhcpPacket p;
    p.setXid(xid);
    QByteArray data;
    udpSocket socket(sd, CLIENT_SOCKET);
    socket.write( ipAddress::full() , SERVER_SOCKET , data);
}

/*!
  Записывает отличительные черты в поток.
  @param stream - поток для записи.
*/
void dhcpClientProgramm::write(QDataStream &stream) const
{
    stream << DHCPClient;
    programmRep::write(stream);
}
//---------------------------------------------------
/*!
  Считывает отличительные черты из потока.
  @param stream - поток для чтения.
*/
void dhcpClientProgramm::read(QDataStream &stream)
{
    programmRep::read(stream);
}
//---------------------------------------------------
