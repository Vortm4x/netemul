#include "frame.h"
#include "arppacket.h"
#include "ippacket.h"
#include <QtTest>

class TestFrame : public QObject
{
    Q_OBJECT
private slots:
    void saveLoadArp();
    void saveLoadIp();
    void creatingCopy();
private:
    Frame arpFrame;
    Frame ipFrame;
};

void TestFrame::saveLoadArp()
{
    ArpPacket p;
    p.setType(ArpPacket::response);
    p.setReceiverIp(IpAddress("1.2.3.4"));
    p.setReceiverMac(MacAddress("10:01:10:01:10:01"));
    p.setSenderIp(IpAddress("4.3.2.1"));
    p.setSenderMac(MacAddress("00:11:22:33:44:55"));
    arpFrame.pack(p.toData());
    ArpPacket a(arpFrame.unpack() );
    QCOMPARE( p.type() , a.type() );
    QCOMPARE( p.receiverIp() , a.receiverIp() );
    QCOMPARE( p.receiverMac() ,a.receiverMac() );
    QCOMPARE( p.senderIp() , a.senderIp() );
    QCOMPARE( p.senderMac() , a.senderMac() );
}

void TestFrame::saveLoadIp()
{
    IpPacket p;
    p.setSender(IpAddress("1.2.3.4"));
    p.setReceiver(IpAddress("4.3.2.1"));
    p.setUpProtocol(IpPacket::tcp);
    ipFrame.pack( p.toData() );
    IpPacket a( ipFrame.unpack() );
    QCOMPARE( p.sender() , a.sender() );
    QCOMPARE( p.receiver() , a.receiver() );
    QCOMPARE( p.upProtocol() , a.upProtocol() );
}

void TestFrame::creatingCopy()
{
    arpFrame.setSender(tr("09:09:09:09:09:09"));
    ipFrame.setSender( arpFrame.sender() );
    Frame a = arpFrame;
    Frame b = ipFrame;
    QCOMPARE( a.sender() , b.sender() );
}

QTEST_MAIN(TestFrame)
#include "main.moc"

