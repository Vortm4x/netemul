#include <QtTest>
#include "../../src/chips/boxchip.h"

class TestBoxChip : public QObject
{
   Q_OBJECT
private slots:
    void setSocketsCount();
};

void TestBoxChip::setSocketsCount()
{
    boxChip b(4);
    QCOMPARE(b.socketsCount() , 4);
    b.setSocketsCount(2);
    QCOMPARE(b.socketsCount() , 2);
    b.setSocketsCount(12);
    QCOMPARE(b.socketsCount() , 12);
}

QTEST_MAIN(TestBoxChip)
#include "main.moc"
