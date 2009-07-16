#ifndef SHAREBUS_H
#define SHAREBUS_H

#include "device.h"
class QPainter;
class devicePort;

// Этот класс представляет собой общую шину, он тоже вынесен в отдельный класс
// который унаследован от device , так как у общей шины будет отличаться
// реализация и отображение от остальных устройств
//
class shareBus : public deviceImpl
{
    Q_DECLARE_TR_FUNCTIONS(shareBus)
public:
    enum { Type = UserType + 8 , busDev = 2};
    enum { defaultWidth = 200 };
    int type() const { return Type; }
    shareBus(int c);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option,QWidget *widget);
    QPointF getPointCable(QPointF otherDev);
    devicePort* addInterface(QString str,int t);
    void dialog() { }
    QString hasTable() const { return QObject::tr(""); }
private:
    int widthDev; // Это длина нашего устройства общей шины
    //и она может изменяться в отличии от других устройств
protected:
    void write(QDataStream &stream) const;
    void read(QDataStream &stream);
};

#endif // SHAREBUS_H
