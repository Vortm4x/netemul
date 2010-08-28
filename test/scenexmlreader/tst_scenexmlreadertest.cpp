#include <QtCore/QString>
#include <QtTest/QtTest>

#include <QXmlSimpleReader>
#include "scenexmlreader.h"
#include "mycanvas.h"
#include "computer.h"

class ScenexmlreaderTest : public QObject
{
    Q_OBJECT

public:
    ScenexmlreaderTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void startElement();        
private:
};

ScenexmlreaderTest::ScenexmlreaderTest()
{
}

void ScenexmlreaderTest::initTestCase()
{
}

void ScenexmlreaderTest::cleanupTestCase()
{
}

void ScenexmlreaderTest::startElement()
{
    QFile file("test1.xml");
    if ( !file.open(QIODevice::ReadOnly) ) {
        QFAIL("can't open file!");
    }
    QXmlInputSource source(&file);
    QXmlSimpleReader reader;
    MyCanvas canvas;
    SceneXmlReader handler(&canvas);
    reader.setContentHandler( &handler );

    QCOMPARE( reader.parse(source) , true );
    QCOMPARE( canvas.devices().size() , 2 );
    QCOMPARE( canvas.devices().at(0)->x() , 475 );
    QCOMPARE( canvas.devices().at(1)->y() , 175 );
    QVERIFY( canvas.devices().at(1)->deviceImpl() != 0 );
    Computer *c = qobject_cast<Computer*>(canvas.devices().at(1)->deviceImpl());
    QVERIFY( c->interfaces().at(0) != 0 );
    QCOMPARE( c->interfaces().at(0)->ip() , QString("192.168.1.131") );
}

QTEST_APPLESS_MAIN(ScenexmlreaderTest);

#include "tst_scenexmlreadertest.moc"
