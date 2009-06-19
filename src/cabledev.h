#ifndef CABLEDEV_H
#define CABLEDEV_H

#include <QGraphicsLineItem>

class device;
class devicePort;
class frame;

//
// Этот класс содержит кабеля которыми мы соединяем устройства
//
class cableDev : public QObject, public QGraphicsLineItem
{
public:
    enum { Type = UserType + 1 , hub = UserType + 7 };
    int type() const { return Type; }
    QRectF boundingRect() const {
        return QRectF(line().p1(),line().p2()).normalized().adjusted(-5,-5,5,5);
    }
    cableDev(device *start,device *end,devicePort* startInter, devicePort* endInter,int s = 5); // Конструктору нужны начала и конец кабеля
    ~cableDev();
    void updatePosition(); // Обновление прорисовки
    void motion();
    bool isBusy() const { return myFrames.count() > 0 ; }
    device* start() { return myStartDev; }
    device* end() { return myEndDev; }
    devicePort* startPort() { return myStartPort; }
    devicePort* endPort() { return myEndPort; }
    void addFrame(frame *fr) {  myFrames << fr;  }
    void removeFrame(frame *fr) { myFrames.removeOne(fr); }
    QList<frame*> frames() { return myFrames; }
    int model() { return myModel; }
    void input(frame *fr,devicePort *cur);
    void output(frame *fr);
private:
    int myModel;
    QList<frame*> myFrames;
    device *myStartDev; // Указатель на объект начала
    device *myEndDev; // и конца этого кабеля
    devicePort *myStartPort;
    devicePort *myEndPort;
    int mySpeed;
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                QWidget *widget = 0); // Как будем рисовать
    //void timerEvent(QTimerEvent *e);
};

#endif // CABLEDEV_H
