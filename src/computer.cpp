#include "computer.h"
#include "cabledev.h"
#include "interface.h"
#include "computerproperty.h"
#include <QPainter>
#include <QPicture>
#include <QtDebug>


computer::computer(int c)
{
    count = 0;
    for ( int i = 0 ; i < c ; i++)
        addInterface(QString("eth%1").arg(count++) , interface::ethernet100 );
    addToTable(ipAddress("127.0.0.0"),ipAddress("255.0.0.0"),ipAddress("127.0.0.1"),ipAddress("127.0.0.1"),0,0,
               connectMode);
}

void computer::paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget)
{
    Q_UNUSED(option); // Пока не используем
    Q_UNUSED(widget); // Эти параметры
    QRect temp(device::rectDevX,device::rectDevY,device::rectDevWidth,device::rectDevHeight); // Временный прямоугольник
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
        tempGrad.setColorAt(1,Qt::lightGray);
    }
    painter->setBrush(QBrush(tempGrad));
    painter->drawRoundedRect(temp,5,5); //Рисуем край нашего компьютера
    painter->drawPixmap(temp,QPixmap(":/im/images/laptop.png")); // Потом картинку
    if ( isConnect() ) painter->setBrush(Qt::green) ; else painter->setBrush(Qt::red);
    painter->drawEllipse(-17,-17,6,6);
}

QPointF computer::getPointCable(QPointF otherDev)
{
    Q_UNUSED(otherDev);
    return pos();
}

devicePort* computer::addInterface(QString str,int t)
{
    devicePort *tempPort = new devicePort;
    interface *tempInter = new interface(tempPort, t);
    tempInter->setSmart(this);
    tempPort->setConnect(false, NULL);
    tempPort->setParentDev(tempInter);
    tempPort->setName(str);
    addSocket(tempPort);
    return tempPort;
}

void computer::write(QDataStream &stream) const
{
    stream << compDev;
    smartDevice::write(stream);
}

void computer::read(QDataStream &stream)
{
    smartDevice::read(stream);
}

void computer::dialog()
{
    computerProperty *d = new computerProperty;
    d->setComputer(this);
    d->exec();
    delete d;
}


