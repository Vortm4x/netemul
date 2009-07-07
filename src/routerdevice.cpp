#include "routerdevice.h"
#include "routerproperty.h"
#include "ripprogramm.h"

routerDevice::routerDevice(int c)
{
    int i;
    for ( i = 1 ; i <=  c ; i++ ) {
        QString t = trUtf8("LAN%1").arg(i);
        addInterface(t,0);
    }
}

devicePort* routerDevice::addInterface(QString str,int t)
{
    devicePort *tempPort = new devicePort;
    interface *tempInter = new interface(tempPort, t);
    tempInter->setSmart(this);
    tempPort->setParentDev(tempInter);
    tempPort->setConnect(false, NULL);
    tempPort->setName(str);
    addSocket(tempPort);
    return tempPort;
}

void routerDevice::receiveEvent(frame *fr , devicePort *sender)
{
    Q_UNUSED(sender);
    delete fr;
}

void routerDevice::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
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
        tempGrad.setColorAt(1,Qt::darkCyan);
    }
    painter->setBrush(QBrush(tempGrad));
    painter->drawRoundedRect(temp,5,5); // Вывести край
    painter->drawPixmap(temp,QPixmap(":/im/images/router.png")); // И рисунок =)
    if ( isConnect() ) {
        if ( myReady == myCableList.count() ) painter->setBrush(Qt::green) ;
        else painter->setBrush(Qt::yellow);
    }
    else painter->setBrush(Qt::red);
    painter->drawEllipse(-17,-17,6,6);
}

void routerDevice::dialog()
{
    routerProperty *d = new routerProperty;
    d->setRouter(this);
    d->exec();
    delete d;
}

void routerDevice::write(QDataStream &stream) const
{
    stream << routerDev;
    smartDevice::write(stream);
}

void routerDevice::read(QDataStream &stream)
{
   smartDevice::read(stream);
}


