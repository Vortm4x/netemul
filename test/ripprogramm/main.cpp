#include <QtTest>
#include "routemodel.h"
#include "ripprogramm.h"
#include "smartdevice.h"
#include "udppacket.h"
#include "interface.h"

class TestRipProgramm : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void execute();
    void sendUpdateAll();
    void sendUpdateNoAll();
    void checkTable();
    void cleanupTestCase();
private:
    smartDevice *device;
    routeModel *model;
    ripProgramm *programm;
    ipPacket packet;
};

void TestRipProgramm::initTestCase()
{
    device = new smartDevice;
    programm = new ripProgramm;
    programm->setDevice(device);
    model = device->routeTable();
    udpPacket t;
    QByteArray b;
    QDataStream s(&b , QIODevice::WriteOnly );
    for ( int i = 1 ; i < 5 ; i++ )
        s << ipAddress(tr("192.168.%1.0").arg(i)) << ipAddress("255.255.255.0") << qint8(1);
    t.pack(b);
    packet.pack(t.toData());
    packet.setSender(tr("192.168.4.63"));
    packet.setReceiver(tr("192.168.1.1"));
}

void TestRipProgramm::execute()
{
    programm->execute(packet);
    QCOMPARE( model->rowCount() , 4 );
}

void TestRipProgramm::sendUpdateAll()
{
    programm->execute(packet);
    programm->sendUpdate(true);
    QCOMPARE( model->rowCount() , 4 );
    foreach ( interface *i , device->interfaces() )  {
        QByteArray b = i->buffer();
        if ( i->ip() == ipAddress("192.168.4.1") ) {
            QCOMPARE( b.size() , 0 );
            continue;
        }
        QCOMPARE( b.size() / 9 , 4 );
    }
}

void TestRipProgramm::sendUpdateNoAll()
{
    programm->execute(packet);
    routeRecord *t = model->recordAt(tr("192.168.2.0"));
    t->change = routeModel::changed;
    QCOMPARE ( programm->interrupt( smartDevice::addNet ) , true );
    programm->sendUpdate(false);
    QCOMPARE( model->rowCount() , 4 );
    foreach ( interface *i , device->interfaces() )  {
        QByteArray b = i->buffer();
        if ( i->ip() == ipAddress("192.168.4.1") ) {
            QCOMPARE( b.size() / 9 , 0 );
            continue;
        }
        QCOMPARE( b.size() / 9 , 1 );
    }
    t->change = routeModel::noChanged;
}

void TestRipProgramm::checkTable()
{
    QCOMPARE( model->rowCount() , 4);
    routeRecord *r = new routeRecord;
    r->dest = tr("0.0.0.0");
    r->mask = tr("0.0.0.0");
    r->metric = 16;
    programm->checkTable(r);
    QCOMPARE( model->rowCount() , 4);
}

void TestRipProgramm::cleanupTestCase()
{
    delete device;
    delete programm;
}



QTEST_MAIN(TestRipProgramm)
#include "main.moc"
