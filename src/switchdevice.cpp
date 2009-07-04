#include "switchdevice.h"
#include "switchproperty.h"
#include "tableswitch.h"
#include "frame.h"

switchDevice::switchDevice(int c)
{
    int i;
    for ( i = 1 ; i <=  c ; i++ ) {
        QString t = trUtf8("LAN%1").arg(i);
        addInterface(t,0);
    }
    mySwitchTable.clear();
}

switchDevice::~switchDevice()
{
    qDeleteAll(mySwitchTable);
    mySwitchTable.clear();
}

// Как было сказано ранее все на примере свитча
void switchDevice::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QRect temp(device::rectDevX,device::rectDevY,device::rectDevWidth,device::rectDevHeight); // Временный треугольник
    QList<QGraphicsItem*> collides = collidingItems();
    foreach ( QGraphicsItem* item , collides) {
        if ( item->type() == cableDev::Type || item->type() == QGraphicsLineItem::Type) collides.removeOne(item);
    }
    QLinearGradient tempGrad(device::rectDevX , device::rectDevY ,-device::rectDevX,-device::rectDevY);
    tempGrad.setColorAt(0,Qt::white);
    if (isSelected()) {
        if (!collides.isEmpty())
            tempGrad.setColorAt(1,Qt::red);
        else
            tempGrad.setColorAt(1,Qt::blue);
        painter->setPen(Qt::darkBlue);
    }
    else {
        painter->setPen(Qt::black); // А иначе черный
        tempGrad.setColorAt(1,Qt::green);
    }
    painter->setBrush(QBrush(tempGrad));
    painter->drawRoundedRect(temp,5,5); // Вывести край
    painter->drawPixmap(temp,QPixmap(":/im/images/switch.png")); // И рисунок =)
    if ( isConnect() ) painter->setBrush(Qt::green) ; else painter->setBrush(Qt::red);
    painter->drawEllipse(-17,-17,6,6);
}

void switchDevice::receiveEvent(frame *fr,devicePort *sender)
{
    bool contains = false;
    foreach ( macRecord *i , mySwitchTable )
        if ( i->mac == fr->sender() ) {
            if ( i->port != sender && i->mode != staticMode ) i->port = sender;
            contains = true;
            break;
        }
    if ( !contains ) addToTable(fr->sender() , sender->name() , dinamicMode , 0);

    foreach ( macRecord *i , mySwitchTable )
        if ( i->mac == fr->receiver() ) {
            if ( i->port->isConnect() && i->port != sender ) {
                    i->port->addToQueue(fr);
            }
            return;
        }
    foreach ( devicePort *item , mySockets ) {
        if ( item != sender && item->isConnect() ) {
            frame *temp = new frame;
            *temp = *fr;
            item->addToQueue(temp);
        }
    }
    delete fr;
}

devicePort* switchDevice::addInterface(QString str,int t)
{
        Q_UNUSED(t);
        devicePort *temp = new devicePort;
        temp->setParentDev(this);
        temp->setName(str);
        addSocket(temp);
        return temp;
}

void switchDevice::write(QDataStream &stream) const
{
    int k;
    stream << switchDev << pos() << sockets().count();
    stream << myType;
    stream << myMac;
    stream << myIp;
    stream << myMask;
    stream << myReceiveFrame;
    stream << mySendFrame;
    stream << myReceivePacket;
    stream << mySendPacket;
    k = mySwitchTable.count();
    stream << k;
    if (k > 0) {
        foreach (macRecord *i, mySwitchTable)
            stream << i->mac << i->port->name() << i->mode << i->time;
     }
}

void switchDevice::read(QDataStream &stream)
{
    QPointF p;
    int n, i, t;
    macAddress m;
    QString value;
    stream >> p >> n;
    setPos(p);
    setSocketCount(n);
    stream >> myType;
    stream >> myMac;
    stream >> myIp;
    stream >> myMask;
    stream >> myReceiveFrame;
    stream >> mySendFrame;
    stream >> myReceivePacket;
    stream >> mySendPacket;
    stream >> t;
    for ( i = 0 ; i < t ; i++) {
        macRecord *r = new macRecord;
        stream >> r->mac;
        stream >> value >> r->mode >> r->time;
        r->port = socket(value);
        mySwitchTable << r;
    }
}

void switchDevice::dialog()
{
    switchProperty *d = new switchProperty;
    d->setSwitch(this);
    d->exec();
    delete d;
}

void switchDevice::dialogTable()
{
    tableSwitch *d = new tableSwitch;
    d->setSwitch(this);
    d->exec();
    delete d;
}

macRecord* switchDevice::addToTable(macAddress mac ,QString name , int mode , int time)
{
    foreach ( macRecord *i , mySwitchTable )
        if ( i->mac == mac ) return NULL;
    macRecord *t = new macRecord;
    t->time = time;
    t->mode = mode;
    t->port = socket(name) ;
    t->mac = mac;
    mySwitchTable << t;
    return t;
}

void switchDevice::deleteFromTable(macAddress mac)
{
    foreach ( macRecord *i , mySwitchTable )
        if ( i->mac == mac ) {
            mySwitchTable.removeOne(i);
            delete i;
            return;
        }
}

void switchDevice::updateMac(int u)
{
    foreach( macRecord *i, mySwitchTable)
        if ( ++i->time == u ) deleteFromTable(i->mac);
}
