#include <QtTest>
#include "../../src/ippacket.h"
#include "../../src/ipaddress.h"
#include "../../src/tcppacket.h"
#include "../../src/udppacket.h"

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
    ipPacket tcpIp;
    ipPacket udpIp;
};
//---------------------------------------
/*!
  Тестирует setBroadcast & isBroadcast
*/
void TestIpPacket::setBroadcast()
{
    ipPacket p;
    p.setReceiver(ipAddress("192.168.1.13"));
    QCOMPARE(p.isBroadcast("255.255.0.0") , false );
    p.setSender(ipAddress("192.168.1.2"));
    p.setBroadcast("255.255.255.224");
    QCOMPARE( p.isBroadcast("255.255.255.224") , true );
    p.setSender(ipAddress("192.168.1.255"));
    p.setBroadcast("255.255.255.0");
    QCOMPARE( p.isBroadcast("255.255.255.0") , true);
    ipPacket a = p;
    ipPacket b = p;
    QCOMPARE( a.sender() , b.sender() );
}
//------------------------------------------

void TestIpPacket::saveLoadTcp()
{
    tcpPacket *p = new tcpPacket;
    p->setSender(1024);
    p->setReceiver(512);
    tcpIp.pack(p->toData());
    delete p;
    p = new tcpPacket( tcpIp.unpack() );
    QCOMPARE( p->sender() , quint16(1024) );
    QCOMPARE( p->receiver() ,quint16( 512 ));
    delete p;
}

void TestIpPacket::saveLoadUdp()
{
    udpPacket *p = new udpPacket;
    p->setSender(1024);
    p->setReceiver(512);
    udpIp.pack( p->toData() );
    delete p;
    p = new udpPacket( udpIp.unpack() );
    QCOMPARE( p->sender() , quint16(1024) );
    QCOMPARE( p->receiver() , quint16(512) );
    delete p;
}

QTEST_MAIN(TestIpPacket)
#include "main.moc"
