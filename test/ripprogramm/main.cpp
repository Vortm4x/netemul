#include <QtTest>
#include "routemodel.h"
#include "ripprogramm.h"
#include "smartdevice.h"
#include "udppacket.h"

class TestRipProgramm : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void execute();
    void sendUpdate();
    void cleanupTestCase();
private:
    smartDevice *device;
    routeModel *model;
    ripProgramm *programm;
};

void TestRipProgramm::initTestCase()
{
    device = new smartDevice;
    programm = new ripProgramm;
    programm->setDevice(device);
    model = device->routeTable();
}

void TestRipProgramm::execute()
{
    udpPacket t;
    QByteArray b;
    QDataStream s(&b , QIODevice::WriteOnly );
    for ( int i = 1 ; i < 5 ; i++ )
        s << ipAddress(tr("192.168.%1.0").arg(i)) << ipAddress("255.255.255.0") << qint8(1);
    t.pack(b);
    ipPacket p;
    p.pack(t.toData());
    p.setSender(tr("192.168.1.63"));
    p.setReceiver(tr("192.168.1.1"));
    programm->execute(p);
    QCOMPARE( model->rowCount() , 4 );
}

void TestRipProgramm::sendUpdate()
{

}

void TestRipProgramm::cleanupTestCase()
{
    delete device;
    delete programm;
}



QTEST_MAIN(TestRipProgramm)
#include "main.moc"
