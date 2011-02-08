#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>

#include "../../src/graphics/cabledev.h"
#include "device.h"

class CableTest : public QObject
{
    Q_OBJECT

public:    

private Q_SLOTS:
  //  void testCase1();
};

/*
void CableTest::testCase1()
{
    Cable *cable = new Cable();
    Device *dev1 = new Device();
    dev1->addPort(new DevicePort("LAN1"));
    Device *dev2 = new Device();
    dev2->addPort(new DevicePort("LAN2"));
    cable->insertInPort( dev1->findPortByName("LAN1") );
    cable->insertInPort( dev2->findPortByName("LAN2") );
    QCOMPARE( cable->startPort() , dev1->findPortByName("LAN1") );
    QCOMPARE( cable->endPort() , dev2->findPortByName("LAN2") );

}
*/

QTEST_MAIN(CableTest);

#include "tst_cabletest.moc"
