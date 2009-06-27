#include <QtTest>
#include "../../src/macaddress.h"

class TestMacAddress : public QObject
{
    Q_OBJECT
private slots:
    void macString();
    void setBroadcast();
    void saveAndLoad();
    void operators();
};

void TestMacAddress::macString()
{
    macAddress m;
    m.setMac("01:03:00:00:00:00");
    QCOMPARE(m.macString(), tr("01:03:00:00:00:00"));
}

void TestMacAddress::setBroadcast()
{
    macAddress m;
    m.setBroadcast();
    QCOMPARE( m.macString() , tr("FF:FF:FF:FF:FF:FF"));
    QCOMPARE( m.isBroadcast() , true );
}

void TestMacAddress::saveAndLoad()
{
    macAddress m("01:02:03:04:05:06");
    macAddress t = m;
    QFile f("test3");
    if ( f.open(QIODevice::WriteOnly) ) {
        QDataStream s(&f);
        s.setVersion(QDataStream::Qt_4_3);
        s << t;
        f.close();
    }
    t.setRandom();
    if ( f.open(QIODevice::ReadOnly) ) {
        QDataStream s(&f);
        s.setVersion(QDataStream::Qt_4_3);
        s >> t;
        f.close();
    }
    QCOMPARE( t , m);
}

void TestMacAddress::operators()
{
    macAddress a("01:02:03:04:05:06");
    macAddress b("01:02:03:04:05:07");
    QCOMPARE( a < b  , true );
    QCOMPARE( a >= b , false );
    QCOMPARE( a++ == b++ , false );
    QCOMPARE( ++a == b , true );
}

QTEST_MAIN(TestMacAddress)
#include "main.moc"
