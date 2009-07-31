#include "../../src/frame.h"
#include "../../src/arppacket.h"
#include "../../src/ippacket.h"
#include <QtTest>

class TestFrame : public QObject
{
    Q_OBJECT
private slots:
    void saveLoadArp();
    void saveLoadIp();
    void creatingCopy();
private:
    frame arpFrame;
    frame ipFrame;
};

void TestFrame::saveLoadArp()
{
    arpPacket p;
    p.setType(arpPacket::response);
    p.setReceiverIp(ipAddress("1.2.3.4"));
    p.setReceiverMac(macAddress("10:01:10:01:10:01"));
    p.setSenderIp(ipAddress("4.3.2.1"));
    p.setSenderMac(macAddress("00:11:22:33:44:55"));
    arpFrame.pack(p.toData());
    arpPacket a(arpFrame.unpack() );
    QCOMPARE( p.type() , a.type() );
    QCOMPARE( p.receiverIp() , a.receiverIp() );
    QCOMPARE( p.receiverMac() ,a.receiverMac() );
    QCOMPARE( p.senderIp() , a.senderIp() );
    QCOMPARE( p.senderMac() , a.senderMac() );
}

void TestFrame::saveLoadIp()
{
    ipPacket p;
    p.setSender(ipAddress("1.2.3.4"));
    p.setReceiver(ipAddress("4.3.2.1"));
    p.setUpProtocol(ipPacket::tcp);
    ipFrame.pack( p.toData() );
    ipPacket a( ipFrame.unpack() );
    QCOMPARE( p.sender() , a.sender() );
    QCOMPARE( p.receiver() , a.receiver() );
    QCOMPARE( p.upProtocol() , a.upProtocol() );
}

void TestFrame::creatingCopy()
{
    arpFrame.setSender(tr("09:09:09:09:09:09"));
    ipFrame.setSender( arpFrame.sender() );
    frame a = arpFrame;
    frame b = ipFrame;
    QCOMPARE( a.sender() , b.sender() );
}

QTEST_MAIN(TestFrame)
#include "main.moc"

