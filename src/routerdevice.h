#ifndef ROUTERDEVICE_H
#define ROUTERDEVICE_H

#include "smartdevice.h"

class routerDevice : public smartDevice
{
    Q_DECLARE_TR_FUNCTIONS(routerDevice)
public:
    enum { Type = UserType + 8 , routerDev = 7 };
    int type() const { return Type; }
    routerDevice(int c);
    devicePort* addInterface(QString str,int t);
    void receiveEvent(frame *fr , devicePort *sender);
    void treatPacket(ipPacket *p);
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget);
    QPointF getPointCable(QPointF otherDev) { Q_UNUSED(otherDev); return pos(); }
    void dialog();
    QString hasTable() const { return trUtf8("Таблица маршрутизации"); }
protected:
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
};

#endif // ROUTERDEVICE_H
