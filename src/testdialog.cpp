#include "testdialog.h"
#include <QtDebug>
#include <QFile>
#include <QDataStream>
#include "mycanvas.h"
#include "cabledev.h"
#include "computer.h"
#include "deviceport.h"
#include "switchdevice.h"
#include "macaddress.h"
#include "hubdevice.h"
#include "frame.h"
#include "routerdevice.h"
#include "ippacket.h"


#define ASSERT(A,B) if(!(A)){ qDebug() << "Line #"<< __LINE__ ; B = false; }
#define TEST(A) how = A; if (!b || !how) break;



testDialog::testDialog(myCanvas *c,QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    canva = c;
    listWidget->setCurrentRow(0);
    label->setText(tr("click start"));
    connect( startButton , SIGNAL(clicked()), this , SLOT(start()));
}



testDialog::~testDialog()
{

}

void testDialog::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}


void testDialog::start()
{
    bool how = true;
    startButton->setEnabled(false);
    canva->killTimer( canva->timerId() );
    if ( canva->isOpen() ) canva->closeFile();
    bool b = checkBox->isChecked();
    int t = (b) ? 0 : listWidget->currentRow() ;
    switch (t) {
        case 0: TEST(test1())
        case 1: TEST(test2())
        case 2: TEST(test3())
        case 3: TEST(test4())
        case 4: TEST(test5())
        case 5: TEST(test6())
        case 6: TEST(test7())
        case 7: TEST(test8())
        case 8: TEST(test9())
        default:
            qDebug() << "Testing finished";
            break;
    }
    if ( how ) {
        label->setText(trUtf8("Testing is correct"));
        label->setStyleSheet("background : green");
    } else {
        label->setText(trUtf8("Testing is not correct"));
        label->setStyleSheet("background : red");
    }
    startButton->setEnabled(true);
    canva->timerId() = canva->startTimer(100);
    if (canva->isOpen() ) canva->closeFile();
}

bool testDialog::test1()
{
    bool res = true;
    qDebug() << "test1 started";
    canva->newFile();
    computer *mas[4];
    mas[0] = canva->createDev<computer>(0,0);
    mas[1] = canva->createDev<computer>(5,0);
    mas[2] = canva->createDev<computer>(0,5);
    mas[3] = canva->createDev<computer>(5,5);
    switchDevice *center =  canva->createDev<switchDevice>(3,3,4);
    center->setMode(abstractChip::test);
    int i = 0;
    QString str = tr("eth0");
    foreach ( devicePort *t , center->sockets() ) {
        canva->createConnection( center, mas[i] , t->name() , str );
        i++;
    }
    macAddress m;
    m.setBroadcast();
    for ( i = 0 ; i < 4 ; i++)  {
        frame *tempFrame = new frame;
        tempFrame->setSender( mas[i]->adapter(str)->mac() );
        tempFrame->setReceiver( m );
        mas[i]->socket(str)->sendFrame(tempFrame);
    }
    canva->whileMotion();
    int sumP = 0, sumS = 0;
    for ( i = 0 ; i < 4 ; i++) {
        sumP += mas[i]->adapter(str)->countRecFrame();
        sumS += mas[i]->adapter(str)->countSendFrame();
    }
    ASSERT( sumS == 4 && sumP == 12 , res );
    canva->saveScene(trUtf8("test1.net"));
    canva->closeFile();
    canva->openScene(trUtf8("test1.net"));
    i = 0;
    foreach ( QGraphicsItem *t , canva->items() ) {
        if ( t->type() == switchDevice::Type || t->type() == cableDev::Type) continue;
        mas[i++] = qgraphicsitem_cast<computer*>(t);
    }
    for ( i = 0 ; i < 4 ; i++)  {
        frame *tempFrame = new frame;
        tempFrame->setSender( mas[i]->adapter(str)->mac() );
        tempFrame->setReceiver( mas[3-i]->adapter(str)->mac() );
        mas[i]->socket(str)->sendFrame(tempFrame);
    }
    canva->whileMotion();
    sumP = 0; sumS = 0;
    for ( i = 0 ; i < 4 ; i++) {
        sumP += mas[i]->adapter(str)->countRecFrame();
        sumS += mas[i]->adapter(str)->countSendFrame();
    }
    qDebug() << sumP << " = " << sumS;
    qDebug() << "test1 finished\n";
    canva->closeFile();
    ASSERT(sumS+8 == sumP,res);
    return res;
}

bool testDialog::test2()
{
    qDebug() << "test2 started";
    canva->newFile();
    int i;
    computer *mas[22];
    for ( i = 0 ; i < 22 ; i++)
    {
        mas[i] = canva->createDev<computer>(i,0);
    }
    switchDevice *sw = canva->createDev<switchDevice>(7,7,12);
    hubDevice *hb = canva->createDev<hubDevice>(19,7,12);
    sw->setMode(abstractChip::test);
    hb->setMode(abstractChip::test);
    canva->createConnection( sw, hb , tr("LAN12") , tr("LAN12") );
    for ( i = 1 ; i <= 11 ; i++ ) {
        canva->createConnection( sw , mas[i-1] , tr("LAN%1").arg(i) , tr("eth0"));
        canva->createConnection( hb,mas[i-1+11], tr("LAN%1").arg(i) , tr("eth0"));
    }
    canva->saveScene(trUtf8("test2.net"));
    macAddress m;
    m.setBroadcast();
    for ( i = 0 ; i < 50 ; i++)  {
        frame *tempFrame = new frame;
        tempFrame->setSender( mas[0]->adapter("eth0")->mac() );
        tempFrame->setReceiver( m );
        mas[0]->socket(tr("eth0"))->sendFrame(tempFrame);
    }
    canva->whileMotion();
    int sum = 0;
    for ( i = 0 ; i < 22 ; i++)
        sum += mas[i]->adapter("eth0")->countRecFrame();
    qDebug() << "sum = " << sum;
    qDebug() << "test2 finished\n";
    canva->closeFile();
    bool res = true;
    ASSERT(sum == 1050,res );
    return res;
}

bool testDialog::test3()
{
    qDebug() << "test3 started";
    bool res = true;
    quint8 a[6] = { 0x01 , 0x02 , 0xFF , 0xFF , 0xFF , 0xFF };
    quint8 b[6] = { 0x01 , 0x02 , 0xFF , 0x03 , 0xFF , 0x01 };
    macAddress macA = macAddress(a);
    macAddress macB = macAddress(b);
    macAddress macC;
    macC.setMac("01:03:00:00:00:00");
    macAddress temp = macC;
    QFile f("test3");
    if ( f.open(QIODevice::WriteOnly) ) {
        QDataStream s(&f);
        s.setVersion(QDataStream::Qt_4_3);
        s << temp;
        f.close();
    }
    temp.setRandom();
    if ( f.open(QIODevice::ReadOnly) ) {
        QDataStream s(&f);
        s.setVersion(QDataStream::Qt_4_3);
        s >> temp;
        f.close();
    }
    ASSERT( temp == macC,res);
    ASSERT( macA >= macB , res);
    ASSERT( macC > macB, res);
    ASSERT( macB <= macA,res);
    ASSERT( macB < macC ,res);
    ASSERT( ++macA == macC,res);
    ASSERT( macA != macB,res);
    ASSERT( macA++ == macC,res);
    ASSERT( macA > macC,res);
    qDebug() << "res = " << res;
    qDebug() << "test3 finished\n";
    return res;
}

bool testDialog::test4()
{
    qDebug() << "test4 started";
    bool res = true;
    quint8 a[4] = { 192 , 168 , 1 , 1 };
    quint8 c[4] = { 127 , 0 , 0 , 1 };
    ipAddress ipA = ipAddress(a);
    ipAddress ipB;
    ipB.setIp("192.168.1.1");
    ipAddress ipC;
    ipC.setIp(c);
    ipAddress temp = ipC;
    QFile f("test4");
    if ( f.open(QIODevice::WriteOnly) ) {
        QDataStream s(&f);
        s.setVersion(QDataStream::Qt_4_3);
        s << temp;
        f.close();
    }
    temp.setIp("255.255.255.255");
    if ( f.open(QIODevice::ReadOnly) ) {
        QDataStream s(&f);
        s.setVersion(QDataStream::Qt_4_3);
        s >> temp;
        f.close();
    }
    ASSERT(temp == ipC,res);
    ASSERT( ipA >= ipC,res);
    ASSERT( ipA == ipB ,res);
    ASSERT( ipA != ipC,res);
    ipC = ipA;
    ASSERT( ipC == ipA,res);
    ipA.setIp("192.168.1.1");
    ipB.setIp("255.255.255.192");
    ipC.setIp("192..168.1.65");
    ASSERT( (ipA & ipB) != (ipB & ipC),res);
    ipC.setIp("192.168.1.2");
    ASSERT( (ipA & ipB) == (ipB & ipC),res);
    ipA = ipAddress::full();
    ASSERT( ipA == ipAddress("255.255.255.255"),res);
    quint32 u = ipA.toInt();
    quint32 v = ipC.toInt();
    ASSERT( u == 4294967295U && v == 3232235778U,res);
    ASSERT( !ipA.isEmpty() , res );
    ipA.setIp("0.0.0.0");
    ASSERT( ipA.isEmpty() , res );
    ipA.setIp("255.255.255.255");
    ASSERT( ~ipA == ipAddress("0.0.0.0") , res );
    ipA.setIp("128.0.0.5");
    ASSERT( ~ipA == ipAddress("127.255.255.250") , res );
    qDebug() << "res = " << res;
    qDebug() << "test4 finished\n";
    return res;
}

bool testDialog::test5()
{
    qDebug() << "test5 started";
    canva->newFile();
    computer *mas[4];
    mas[0] = canva->createDev<computer>(0,0);
    mas[1] = canva->createDev<computer>(5,0);
    mas[2] = canva->createDev<computer>(0,5);
    mas[3] = canva->createDev<computer>(5,5);
    routerDevice *center =  canva->createDev<routerDevice>(3,3,4);
    int i = 0;
    QString str = trUtf8("eth0");
    foreach ( devicePort *t , center->sockets() ) {
        canva->createConnection( center, mas[i] , t->name() , str );
        i++;
    }
    macAddress m;
    m.setBroadcast();
    for ( i = 0 ; i < 4 ; i++)  {
        frame *tempFrame = new frame;
        tempFrame->setSender( mas[i]->adapter(str)->mac() );
        tempFrame->setReceiver( m );
        mas[i]->socket(str)->sendFrame(tempFrame);
    }
    canva->whileMotion();
    canva->saveScene(trUtf8("test5.net"));
    canva->closeFile();
    canva->openScene(trUtf8("test5.net"));
    i = 0;
    foreach ( QGraphicsItem *t , canva->items() ) {
        if ( t->type() == routerDevice::Type || t->type() == cableDev::Type) continue;
        mas[i++] = qgraphicsitem_cast<computer*>(t);
    }
    for ( i = 0 ; i < 4 ; i++)  {
        frame *tempFrame = new frame;
        tempFrame->setSender( mas[i]->adapter(str)->mac() );
        tempFrame->setReceiver( mas[3-i]->adapter(str)->mac() );
        mas[i]->socket(str)->sendFrame(tempFrame);
    }
    while ( !canva->isEnd() ) canva->motionFrame();
    int sum = 0;
    for ( i = 0 ; i < 4 ; i++)
        sum += mas[i]->adapter(str)->countRecFrame();
    qDebug() << "sum = " << sum;
    qDebug() << "test5 finished\n";
    canva->closeFile();
    bool res = true;
    ASSERT( sum == 0, res);
    return res;
}

bool testDialog::test6()
{
    bool res = true;
    int i;
    computer *net1[4];
    computer *net2[4];
    qDebug() << "test6 started";
    canva->newFile();
    routerDevice *r = canva->createDev<routerDevice>(6,4,4);
    r->adapter("LAN3")->setMode(abstractChip::test);
    r->adapter("LAN4")->setMode(abstractChip::test);
    switchDevice *h = canva->createDev<switchDevice>(4,4,8);
    h->setMode(abstractChip::test);
    switchDevice *s = canva->createDev<switchDevice>( 8,4 , 8);
    s->setMode(abstractChip::test);
    canva->createConnection( r , h , "LAN3" , "LAN8" );
    canva->createConnection( r , s , "LAN4" , "LAN8" );
    for ( i = 0 ; i < 4 ; i++ ) {
        net1[i] = canva->createDev<computer>(1,i+2,1);
        net1[i]->adapter("eth0")->setMode(abstractChip::test);
        net2[i] = canva->createDev<computer>(11,i+2,1);
        net2[i]->adapter("eth0")->setMode(abstractChip::test);
        canva->createConnection( net1[i] ,h ,"eth0" , tr("LAN%1").arg(i+1) );
        canva->createConnection( net2[i] ,s ,"eth0" , tr("LAN%1").arg(i+1) );
        net1[i]->adapter("eth0")->setIp( tr("192.168.1.%1").arg(i+1) );
        net2[i]->adapter("eth0")->setIp( tr("192.168.1.%1").arg(i+65) );
        net1[i]->adapter("eth0")->setMask("255.255.255.192");
        net2[i]->adapter("eth0")->setMask("255.255.255.192");
        net1[i]->setGateway("192.168.1.63");
        net2[i]->setGateway("192.168.1.127");
    }
    r->adapter("LAN3")->setMask("255.255.255.192");
    r->adapter("LAN4")->setMask("255.255.255.192");
    r->adapter("LAN3")->setIp("192.168.1.63");
    r->adapter("LAN4")->setIp("192.168.1.127");
    r->connectedNet( r->socket("LAN3") );
    r->connectedNet( r->socket("LAN4") );
    canva->saveScene(tr("test6.net"));
    ipPacket *p = new ipPacket(net2[0]->adapter("eth0")->ip(),net2[3]->adapter("eth0")->ip());
    net2[0]->adapter("eth0")->sendPacket(p);
    canva->whileMotion();
    int sumS = 0 , sumR = 0;
    for ( i = 0 ; i < 4 ; i++ ) {
        sumS += net2[i]->adapter("eth0")->countSendPacket();
        sumS += net2[i]->adapter("eth0")->countSendFrame();
        sumR += net2[i]->adapter("eth0")->countRecPacket();
        sumR += net2[i]->adapter("eth0")->countRecFrame();
    }
    qDebug() << sumS << " = " << sumR;
    ASSERT( sumS == 4 && sumR == 6 , res)
    net1[0]->sendMessage(net2[0]->adapter("eth0")->ip() , 50 , smartDevice::UDP);
    canva->whileMotion();
    sumS = 0 ; sumR = 0;
    for ( i = 0 ; i < 4 ; i++) {
        sumS += net1[i]->adapter("eth0")->countSendPacket();
        sumS += net1[i]->adapter("eth0")->countSendFrame();
        sumR += net1[i]->adapter("eth0")->countRecPacket();
        sumR += net1[i]->adapter("eth0")->countRecFrame();
    }
    sumS += r->adapter("LAN3")->countSendFrame() + r->adapter("LAN3")->countSendPacket();
    sumR += r->adapter("LAN3")->countRecFrame() + r->adapter("LAN3")->countRecPacket();
    qDebug() << sumS << " = " << sumR;
    ASSERT( sumS == 102 && sumR == 105 ,res);
    canva->closeFile();
    qDebug() << "test6 finished";
    return res;
}

bool testDialog::test7()
{
    bool res = true;
    qDebug() << "test7 started";
    canva->newFile();
    int i,j,n = 0;
    int w = 1;
    computer *net[7][3];
    switchDevice *sw[7];
    routerDevice *rt[4];
    rt[0] = canva->createDev<routerDevice>(4,2,8);
    rt[1] = canva->createDev<routerDevice>(4,6,8);
    rt[2] = canva->createDev<routerDevice>(8,2,8);
    rt[3] = canva->createDev<routerDevice>(8,8,8);
    for ( i = 0 ; i < 4 ; i++ )
        for ( j = 1 ; j <= 8 ; j++)
            rt[i]->adapter(trUtf8("LAN%1").arg(j))->setMode(abstractChip::test);
    canva->createConnection(rt[0],rt[2],"LAN3","LAN3");
    canva->createConnection(rt[1],rt[3],"LAN3","LAN3");
    canva->createConnection(rt[2],rt[3],"LAN4","LAN4");
    canva->createConnection(rt[1],rt[0],"LAN4","LAN4");
    for ( i = 0 ; i < 3; i++) {
        sw[i] = canva->createDev<switchDevice>(2,w,8);
        sw[i]->setMode(abstractChip::test);
        for ( j = 0 ; j < 3 ; j++) {
            net[i][j] = canva->createDev<computer>(0,n,1);
            net[i][j]->adapter("eth0")->setMode(abstractChip::test);
            canva->createConnection(net[i][j],sw[i],"eth0",tr("LAN%1").arg(j+1));
            n++;
        }
        w+=3;
    }
    n = 0;
    w = 1;
    for ( i = 3 ; i < 7; i++) {
        sw[i] = canva->createDev<switchDevice>(10,w,8);
        sw[i]->setMode(abstractChip::test);
        for ( j = 0 ; j < 3 ; j++) {
            net[i][j] = canva->createDev<computer>(12,n,1);
            net[i][j]->adapter("eth0")->setMode(abstractChip::test);
            canva->createConnection(net[i][j],sw[i],"eth0",tr("LAN%1").arg(j+1));
            n++;
        }
        w+=3;
    }
    canva->createConnection(sw[0],rt[0],"LAN8","LAN1");
    canva->createConnection(sw[1],rt[1],"LAN8","LAN1");
    canva->createConnection(sw[2],rt[1],"LAN8","LAN2");
    canva->createConnection(sw[3],rt[2],"LAN8","LAN1");
    canva->createConnection(sw[4],rt[2],"LAN8","LAN2");
    canva->createConnection(sw[5],rt[3],"LAN8","LAN1");
    canva->createConnection(sw[6],rt[3],"LAN8","LAN2");
    int u,g;
    for ( i = 3 , u = 1 , g = 30; i < 7 ; i++ , u += 32 , g+=32)
        for ( j = 0 ; j < 3; j++ ) {
            net[i][j]->adapter("eth0")->setIp(tr("192.168.1.%1").arg(u+j));
            net[i][j]->adapter("eth0")->setMask("255.255.255.224");
            net[i][j]->setGateway(tr("192.168.1.%1").arg(g));
        }
    for ( i = 0 , u = 129 , g =158 ; i < 3 ; i++ , u+=32 , g += 32)
        for ( j = 0 ; j < 3 ; j++ ) {
            net[2-i][j]->adapter("eth0")->setIp(tr("192.168.1.%1").arg(u+j));
            net[2-i][j]->adapter("eth0")->setMask("255.255.255.224");
            net[2-i][j]->setGateway(tr("192.168.1.%1").arg(g));
        }
    for ( i = 0 ; i < 4; i++){
        rt[i]->adapter("LAN1")->setMask("255.255.255.224");
        if ( rt[i]->adapter("LAN2") ) rt[i]->adapter("LAN2")->setMask("255.255.255.224");
        rt[i]->adapter("LAN3")->setMask("255.255.255.248");
        rt[i]->adapter("LAN4")->setMask("255.255.255.248");
    }
    rt[0]->adapter("LAN1")->setIp("192.168.1.222");
    rt[0]->adapter("LAN3")->setIp("192.168.1.238");
    rt[0]->adapter("LAN4")->setIp("192.168.1.230");
    rt[2]->adapter("LAN1")->setIp("192.168.1.30");
    rt[2]->adapter("LAN2")->setIp("192.168.1.62");
    rt[2]->adapter("LAN3")->setIp("192.168.1.237");
    rt[2]->adapter("LAN4")->setIp("192.168.1.246");
    rt[1]->adapter("LAN1")->setIp("192.168.1.190");
    rt[1]->adapter("LAN2")->setIp("192.168.1.158");
    rt[1]->adapter("LAN3")->setIp("192.168.1.249");
    rt[1]->adapter("LAN4")->setIp("192.168.1.229");
    rt[3]->adapter("LAN1")->setIp("192.168.1.94");
    rt[3]->adapter("LAN2")->setIp("192.168.1.126");
    rt[3]->adapter("LAN3")->setIp("192.168.1.250");
    rt[3]->adapter("LAN4")->setIp("192.168.1.245");
    canva->saveScene(tr("test7.net"));
    canva->closeFile();
    qDebug() << "test7 finished";
    return res;
}

bool testDialog::test8()
{
    bool res = true;
    qDebug() << "test8 started";
    QScriptValue c = engine.newQObject(canva);
    engine.globalObject().setProperty("canva",c);
    QFile file("../test/test8.js");
    if ( !file.open(QIODevice::ReadOnly) ) qDebug() << "XAXA";
    QScriptValue t = engine.evaluate(file.readAll());
    if ( t.isError() ) qDebug() << 1;
    qDebug() << t.toString();
    file.close();
    qDebug() << "test8 finished";
    return res;
}

bool testDialog::test9()
{
    qDebug() << "test9 started";
    bool res = true;
    ipPacket *p = new ipPacket;
    p->setReceiver(ipAddress("192.168.1.13"));
    ASSERT( !p->isBroadcast("255.255.0.0") , res );
    p->setSender(ipAddress("192.168.1.2"));
    p->setBroadcast("255.255.255.224");
    ASSERT( p->isBroadcast("255.255.255.224") , res );
    qDebug() << p->receiver();
    p->setSender(ipAddress("192.168.1.255"));
    p->setBroadcast("255.255.255.0");
    ASSERT( p->isBroadcast("255.255.255.0") , res);
    qDebug() << p->receiver();
    delete p;
    qDebug() << "test9 finished\n";
    return res;
}
