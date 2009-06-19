#ifndef HUBDEVICE_H
#define HUBDEVICE_H

#include "boxdevice.h"
#include "deviceport.h"
class frame;


class hubDevice : public boxDevice
{
public:
    enum { Type = UserType + 7, hubDev = 4 };
    int type() const { return Type; }
    hubDevice(int c);
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget);
    QPointF getPointCable(QPointF otherDev) { Q_UNUSED(otherDev); return pos();}
    void receiveEvent(frame *fr, devicePort *sender);
    devicePort* addInterface(QString str,int t);
    void dialog();
    QString hasTable() const { return tr(""); }
protected:
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
};


#endif // HUBDEVICE_H
