#include "dhcpclientprogramm.h"
#include "smartdevice.h"
#include "udpsocket.h"

dhcpClientProgramm::dhcpClientProgramm()
{
    myName = tr("DHCP client");
    hasSetting = false;
    time = 0;
}

void dhcpClientProgramm::setEnable(bool b)
{
    programmRep::setEnable(b);
    if ( hasSetting && !b ) return; // Не нужно спрашивать настройки при выключении или если они уже есть.
    sendDiscover();
}

void dhcpClientProgramm::incTime()
{
    if (  !hasSetting && !(time%FIVE_MINUTE) ) sendDiscover();
}

void dhcpClientProgramm::sendDiscover()
{
    QByteArray data;
    QDataStream s(&data, QIODevice::WriteOnly);
    s << tr("HELLO I'M DHCPClient, i want ip address!");
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
