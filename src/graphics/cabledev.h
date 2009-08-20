#ifndef CABLEDEV_H
#define CABLEDEV_H

#include <QGraphicsLineItem>
#include <QPainter>
#include <QQueue>

class device;
class devicePort;

struct bitStream {
    QByteArray data;
    QBrush color;
    qreal pos;
};

static const int MINIMUM_DEAD = 3;
static const int TIME_BEFORE_DEAD = 10;
static const int PERCENT_DEAD = 25;

class cableDev : public QGraphicsLineItem
{
public:
    enum { Type = UserType + 1 };
    enum { normal = 3 , broadcast = 4 };
    int type() const { return Type; }
    QRectF boundingRect() const {
        return QRectF(line().p1(),line().p2()).normalized().adjusted(-5,-5,5,5);
    }
    bool isCollisionCable() const { return isCollision; }
    cableDev(device *start,device *end,QString sp, QString ep,int s = 5);
    ~cableDev();
    void updatePosition(); // Обновление прорисовки
    void motion();
    bool isBusy() const { return fromStartQueue.count() || fromEndQueue.count(); }
    bool isBusy(const devicePort *d);
    device* start() { return myStartDev; }
    device* end() { return myEndDev; }
    devicePort* startPort() { return myStartPort; }
    devicePort* endPort() { return myEndPort; }
    void insertInPort(devicePort *p);
    int isShared() const { return myShared; }
    void input(QByteArray b,devicePort *cur);;
    void setChecked(bool c) { myChecked = c; update(); }
    bool isChecked() const { return myChecked; }
    QString startSocketName() const;
    QString endSocketName() const;
    void deleteConnect();
    void killRandomPackets(QQueue<bitStream*> stream);
    void killCurrentPackets();
    void startCollision();
private:
    bool isCollision;
    bool myChecked;
    bool myShared;
    QQueue<bitStream*> fromStartQueue;
    QQueue<bitStream*> fromEndQueue;
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
