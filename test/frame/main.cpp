#include "../../src/frame.h"
#include <QtTest>

class TestFrame : public QObject
{
    Q_OBJECT
private slots:
    void saveLoadArp();
    void saveLoadIp();
private:
    frame arpFrame;
    frame ipFrame;
};

void TestFrame::saveLoadArp()
{
    arpPacket *p = new arpPacket;
    p->setType(arpPacket::answer);
    p->setReceiverIp(ipAddress("1.2.3.4"));
    p->setReceiverMac(macAddress("10:01:10:01:10:01"));
    p->setSenderIp(ipAddress("4.3.2.1"));
    p->setSenderMac(macAddress("00:11:22:33:44:55"));
    arpFrame << *p;
    delete p;
    p = new arpPacket;
    arpFrame >> *p;
    QCOMPARE( p->type() , 1 );
    QCOMPARE( p->receiverIp() , ipAddress("1.2.3.4") );
    QCOMPARE( p->receiverMac() ,macAddress("10:01:10:01:10:01") );
    QCOMPARE( p->senderIp() , ipAddress("4.3.2.1") );
    QCOMPARE( p->senderMac() , macAddress("00:11:22:33:44:55") );
    delete p;
}

void TestFrame::saveLoadIp()
{
    ipPacket *p = new ipPacket;
    p->setSender(ipAddress("1.2.3.4"));
    p->setReceiver(ipAddress("4.3.2.1"));
    p->setUpProtocol(ipPacket::tcp);
    ipFrame << *p;
    delete p;
    p = new ipPacket;
    ipFrame >> *p;
    QCOMPARE( p->sender() , ipAddress("1.2.3.4") );
    QCOMPARE( p->receiver() , ipAddress("4.3.2.1") );
    QCOMPARE( p->upProtocol() , (qint8)1 );
    delete p;
}

QTEST_MAIN(TestFrame)
#include "main.moc"

