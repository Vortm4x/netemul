#include <QtTest>
#include "macaddress.h"

class TestmacAddress : public QObject
{
    Q_OBJECT
private slots:
    void toString();
    void setBroadcast();
    void saveAndLoad();
    void operators();
};

void TestmacAddress::toString()
{
    MacAddress m;
    m.setMac("01:03:00:00:00:00");
    QCOMPARE(m.toString(), tr("01:03:00:00:00:00"));
}

void TestmacAddress::setBroadcast()
{
    MacAddress m;
    m.setBroadcast();
    QCOMPARE( m.toString() , tr("FF:FF:FF:FF:FF:FF"));
    QCOMPARE( m.isBroadcast() , true );
}

void TestmacAddress::saveAndLoad()
{
    MacAddress m("01:02:03:04:05:06");
    MacAddress t = m;
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

void TestmacAddress::operators()
{
    MacAddress a("01:02:03:04:05:06");
    MacAddress b("01:02:03:04:05:07");
    QCOMPARE( a < b  , true );
    QCOMPARE( a >= b , false );
    QCOMPARE( a++ == b++ , false );
    QCOMPARE( ++a == b , true );
}

QTEST_MAIN(TestmacAddress)
#include "main.moc"
