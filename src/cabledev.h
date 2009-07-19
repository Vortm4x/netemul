#ifndef CABLEDEV_H
#define CABLEDEV_H

#include <QGraphicsLineItem>
#include <QPainter>

class device;
class devicePort;

struct bitStream {
    QByteArray data;
    QBrush color;
    qreal pos;
    qint8 direct;
};

class cableDev : public QObject, public QGraphicsLineItem
{
public:
    enum { Type = UserType + 1 , hub = UserType + 7 };
    enum { startToEnd = 1 , endToStart = -1 };
    enum { normal = 3 , broadcast = 4 };
    int type() const { return Type; }
    QRectF boundingRect() const {
        return QRectF(line().p1(),line().p2()).normalized().adjusted(-5,-5,5,5);
    }
    cableDev(device *start,device *end,devicePort* startInter, devicePort* endInter,int s = 5); // Конструктору нужны начала и конец кабеля
    ~cableDev();
    void updatePosition(); // Обновление прорисовки
    void motion();
    bool isBusy() const { return myStreams.count(); }
    device* start() { return myStartDev; }
    device* end() { return myEndDev; }
    devicePort* startPort() { return myStartPort; }
    devicePort* endPort() { return myEndPort; }
    int model() const { return myModel; }
    void input(QByteArray &b,devicePort *cur);
    void output(bitStream *t);
    void setChecked(bool c) { myChecked = c; update(); }
    bool isChecked() const { return myChecked; }
    QString startSocketName() const;
    QString endSocketName() const;
private:
    bool myChecked;
    int myModel;
    QList<bitStream*> myStreams;
    device *myStartDev; //!< Указатель на устройтсво начала.
    device *myEndDev; //!< Указатель на устройство конца.
    devicePort *myStartPort;
    devicePort *myEndPort;
    int mySpeed;
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                QWidget *widget = 0); // Как будем рисовать
};

#endif // CABLEDEV_H
