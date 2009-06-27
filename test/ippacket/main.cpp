#include <QtTest>
#include "../../src/ippacket.h"
#include "../../src/ipaddress.h"

/*!
  Тестирует класс ipPacket
*/
class TestIpPacket : public QObject
{
    Q_OBJECT
private slots:
    void setBroadcast();
};
//---------------------------------------
/*!
  Тестирует setBroadcast & isBroadcast
*/
void TestIpPacket::setBroadcast()
{
    ipPacket *p = new ipPacket;
    p->setReceiver(ipAddress("192.168.1.13"));
    QCOMPARE(p->isBroadcast("255.255.0.0") , false );
    p->setSender(ipAddress("192.168.1.2"));
    p->setBroadcast("255.255.255.224");
    QCOMPARE( p->isBroadcast("255.255.255.224") , true );
    p->setSender(ipAddress("192.168.1.255"));
    p->setBroadcast("255.255.255.0");
    QCOMPARE( p->isBroadcast("255.255.255.0") , true);
    delete p;
}
//------------------------------------------
QTEST_MAIN(TestIpPacket)
#include "main.moc"
