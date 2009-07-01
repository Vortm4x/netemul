#ifndef COMPUTER_H
#define COMPUTER_H

#include "smartdevice.h"
#include "deviceport.h"
#include <QDataStream>

class devicePort;

class computer : public smartDevice
{
    Q_DECLARE_TR_FUNCTIONS(computer)
public:
    // Начнем с 5
    enum { Type = UserType + 5 , compDev = 3 };
    int type() const { return Type; }

    computer(int c);

    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget);
    QPointF getPointCable(QPointF otherDev);

    devicePort* addInterface(QString str,int t);

    void treatPacket(ipPacket *p);
    void dialog();
    QString hasTable() const { return trUtf8("Таблица маршрутизации"); }
protected:
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
};

#endif // COMPUTER_H
