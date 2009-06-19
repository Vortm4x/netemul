#include "hubdevice.h"
#include "hubproperty.h"
#include <QPainter>
#include <QtDebug>

hubDevice::hubDevice(int c)
{
    int i;
    setMode(abstractChip::normal);
    for ( i = 1 ; i <=  c ; i++ ) {
        QString t = trUtf8("LAN%1").arg(i);
        addInterface(t,0);
    }
}
// Пока смотреть свитч =)
void hubDevice::paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QRect temp(device::rectDevX,device::rectDevY,device::rectDevWidth,device::rectDevHeight);
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
        tempGrad.setColorAt(1,Qt::yellow);
    }
    painter->setBrush(QBrush(tempGrad));
    painter->drawRoundedRect(temp,5,5);
    painter->drawPixmap(temp,QPixmap(":/im/images/modem.png"));
    if ( isConnect() ) painter->setBrush(Qt::green) ; else painter->setBrush(Qt::red);
    painter->drawEllipse(-17,-17,6,6);
}

void hubDevice::receiveEvent(frame *fr,devicePort *sender)
{
    foreach ( devicePort *item , mySockets ) {
        if ( item != sender && item->isConnect() ) {
            frame *temp = new frame;
            *temp = *fr;
            item->addToQueue(temp);
        }
    }
    delete fr;
}

devicePort* hubDevice::addInterface(QString str,int t)
{
        Q_UNUSED(t);
        devicePort *temp = new devicePort;
        temp->setParentDev(this);
        temp->setName(str);
        addSocket(temp);
        return temp;
}

void hubDevice::read(QDataStream &stream)
{
    QPointF p;
    int n;
    stream >> p >> n;
    setPos(p);
    setSocketCount(n);
    stream >> myType;
    stream >> myMode;
    stream >> myMac;
    stream >> myIp;
    stream >> myMask;
    stream >> myReceiveFrame;
    stream >> mySendFrame;
    stream >> myReceivePacket;
    stream >> mySendPacket;
}

void hubDevice::write(QDataStream &stream) const
{
    stream << hubDev << pos() << sockets().count();
    stream << myType;
    stream << myMode;
    stream << myMac;
    stream << myIp;
    stream << myMask;
    stream << myReceiveFrame;
    stream << mySendFrame;
    stream << myReceivePacket;
    stream << mySendPacket;
}

void hubDevice::dialog()
{
    hubProperty *d = new hubProperty;
    d->setHub(this);
    d->exec();
    delete d;
}
