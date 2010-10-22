#include <QtTest>
#include "../../src/chips/hubchip.h"

class TestHubChip : public QObject
{
   Q_OBJECT
private slots:
    void setSocketsCount();
};

void TestHubChip::setSocketsCount()
{
    HubChip b(0);
    b.setSocketsCount(4);
    QCOMPARE(b.socketsCount() , 4);
    b.setSocketsCount(2);
    QCOMPARE(b.socketsCount() , 2);
    b.setSocketsCount(12);
    QCOMPARE(b.socketsCount() , 12);
}

QTEST_MAIN(TestHubChip)
#include "main.moc"
