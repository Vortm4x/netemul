#include <QtTest>
#include "ippacket.h"
#include "ipaddress.h"
#include "tcppacket.h"
#include "udppacket.h"

/*!
  Тестирует класс ipPacket
*/
class TestIpPacket : public QObject
{
    Q_OBJECT
private slots:
    void setBroadcast();
    void saveLoadTcp();
    void saveLoadUdp();
private:
    IpPacket tcpIp;
    IpPacket udpIp;
};
//---------------------------------------
/*!
  Тестирует setBroadcast & isBroadcast
*/
void TestIpPacket::setBroadcast()
{
    IpPacket p;
    p.setReceiver(IpAddress("192.168.1.13"));
    QCOMPARE(p.isBroadcast("255.255.0.0") , false );
    p.setSender(IpAddress("192.168.1.2"));
    p.setBroadcast("255.255.255.224");
    QCOMPARE( p.isBroadcast("255.255.255.224") , true );
    p.setSender(IpAddress("192.168.1.255"));
    p.setBroadcast("255.255.255.0");
    QCOMPARE( p.isBroadcast("255.255.255.0") , true);
    IpPacket a = p;
    IpPacket b = p;
    QCOMPARE( a.sender() , b.sender() );
}
//------------------------------------------

void TestIpPacket::saveLoadTcp()
{
    TcpPacket *p = new TcpPacket;
    p->setSender(1024);
    p->setReceiver(512);
    tcpIp.pack(p->toData());
    delete p;
    p = new TcpPacket( tcpIp.unpack() );
    QCOMPARE( p->sender() , quint16(1024) );
    QCOMPARE( p->receiver() ,quint16( 512 ));
    delete p;
}

void TestIpPacket::saveLoadUdp()
{
    UdpPacket *p = new UdpPacket;
    p->setSender(1024);
    p->setReceiver(512);
    udpIp.pack( p->toData() );
    delete p;
    p = new UdpPacket( udpIp.unpack() );
    QCOMPARE( p->sender() , quint16(1024) );
    QCOMPARE( p->receiver() , quint16(512) );
    delete p;
}

QTEST_MAIN(TestIpPacket)
#include "main.moc"
