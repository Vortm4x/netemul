#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>

#include "mycanvas.h"

class MycanvasTest : public QObject
{
    Q_OBJECT

public:
    MycanvasTest();

private Q_SLOTS:
    void testCase1();
};

MycanvasTest::MycanvasTest()
{
}

void MycanvasTest::testCase1()
{

}

QTEST_MAIN(MycanvasTest);

#include "tst_mycanvastest.moc"
