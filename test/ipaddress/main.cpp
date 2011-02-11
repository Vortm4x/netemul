#include <QtTest>
#include "ipaddress.h"

class TestIpAddress : public QObject
{
    Q_OBJECT
private slots:
    void saveAndLoad();
    void operators();
    void toInt();
    void isEmpty();
    void toIp();
};

void TestIpAddress::saveAndLoad()
{
    IpAddress ip;
    ip.setIp("192.168.1.1");
    IpAddress t = ip;
    QFile f("test4");
    if ( f.open(QIODevice::WriteOnly) ) {
        QDataStream s(&f);
        s.setVersion(QDataStream::Qt_4_3);
        s << t;
        f.close();
    }
    t.setIp("255.255.255.255");
    if ( f.open(QIODevice::ReadOnly) ) {
        QDataStream s(&f);
        s.setVersion(QDataStream::Qt_4_3);
        s >> t;
        f.close();
    }
    QCOMPARE( t , ip );
}

void TestIpAddress::operators()
{
    IpAddress ipA;
    ipA.setIp("255.255.255.255");
    QCOMPARE( ~ipA == IpAddress("0.0.0.0") , true );
    ipA.setIp("128.0.0.5");
    QCOMPARE( ~ipA == IpAddress("127.255.255.250") , true );
    ipA.setIp("192.168.1.1");
    IpAddress ipB("255.255.255.192");
    IpAddress ipC("192..168.1.65");
    QCOMPARE( (ipA & ipB) != (ipB & ipC), true);
    ipC.setIp("192.168.1.2");
    QCOMPARE( (ipA & ipB) == (ipB & ipC), true);
    ipA = IpAddress::full();
    QCOMPARE( ipA == IpAddress("255.255.255.255"),true);
}

void TestIpAddress::toInt()
{
    IpAddress ipA("255.255.255.255");
    IpAddress ipB("192.168.1.2");
    IpAddress ipC("0.0.0.0");
    quint32 v1 = ipA.toInt();
    quint32 v2 = ipB.toInt();
    quint32 v3 = ipC.toInt();
    QCOMPARE( v1, 4294967295U );
    QCOMPARE( v2, 3232235778U );
    QCOMPARE( v3, 0U );
}

void TestIpAddress::isEmpty()
{
    IpAddress a("1.0.0.0");
    QCOMPARE( a.isEmpty() , false );
    a.setIp("0.0.0.0");
    QCOMPARE( a.isEmpty() , true );
}

void TestIpAddress::toIp()
{
    IpAddress address("192.168.1.1");
    quint32 intIp = address.toInt();
    QCOMPARE( IpAddress(intIp) , address );
    address.setIp("0.0.0.0");
    intIp = address.toInt();
    QCOMPARE( IpAddress(intIp) , address );
    address.setIp("255.255.255.255");
    intIp = address.toInt();
    QCOMPARE( IpAddress(intIp) , address );
}

QTEST_MAIN(TestIpAddress)
#include "main.moc"


