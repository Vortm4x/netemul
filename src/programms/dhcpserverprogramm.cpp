#include "dhcpserverprogramm.h"
#include "smartdevice.h"
#include "udpsocket.h"

dhcpServerProgramm::dhcpServerProgramm()
{
    myName = tr("DHCP server");
}

void dhcpServerProgramm::setDevice(smartDevice *s)
{
    programmRep::setDevice(s);
    receiver = new udpSocket(sd, SERVER_SOCKET);
    receiver->setBind("0.0.0.0");
    connect( receiver , SIGNAL(readyRead(QByteArray)), SLOT(execute(QByteArray)));
}

void dhcpServerProgramm::execute(QByteArray data)
{
    QString str;
    QDataStream s(data);
    s >> str;
    qDebug("RECEIVE MESSAGE: %s",str.toAscii().data() );
}

/*!
  Записывает отличительные черты в поток.
  @param stream - поток для записи.
*/
void dhcpServerProgramm::write(QDataStream &stream) const
{
    stream << DHCPServer;
    programmRep::write(stream);
}
//---------------------------------------------------
/*!
  Считывает отличительные черты из потока.
  @param stream - поток для чтения.
*/
void dhcpServerProgramm::read(QDataStream &stream)
{
    programmRep::read(stream);
}
//---------------------------------------------------
