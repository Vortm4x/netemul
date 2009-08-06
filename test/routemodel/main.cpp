#include <QtTest>
#include "routemodel.h"


class TestRouteModel : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void mainTest();
    void changedRecord();
    void deleteOldRecord();
    void cleanupTestCase();
private:
    routeModel *table;
};

void TestRouteModel::initTestCase()
{
    table = new routeModel(this);
    for ( int i = 0 ; i < 5 ; i++ )
    table->addToTable(tr("192.168.1.%1").arg(i) ,tr("255.255.255.255"),
                      tr("192.168.1.2"),tr("192.168.1.60"), 2, routeModel::ripMode);
}

void TestRouteModel::mainTest()
{
    QCOMPARE( table->rowCount() , 5 );
}

void TestRouteModel::changedRecord()
{
    routeRecord *t = table->recordAt(tr("192.168.1.0"));
    if ( t ) t->change = routeModel::changed;
    t = table->changedRecord();
    QCOMPARE( t->change , (quint8)routeModel::changed );
    table->deleteFromTable(t);
    t = table->changedRecord();
    if ( t ) QCOMPARE( t->change , (quint8)routeModel::changed);
}

void TestRouteModel::deleteOldRecord()
{
    routeRecord *t = table->recordAt(tr("192.168.1.1"));
    t->time = 6;
    table->deleteOldRecord(6);
    QCOMPARE( table->rowCount() , 3 );

}

void TestRouteModel::cleanupTestCase()
{

}

QTEST_MAIN(TestRouteModel)
#include "main.moc"
