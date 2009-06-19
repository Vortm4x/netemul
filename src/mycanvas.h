#ifndef MYCANVAS_H
#define MYCANVAS_H

#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include "macaddress.h"
#include "ipaddress.h"
#include "interface.h"
#include "hubdevice.h"
#include "switchdevice.h"
#include "routerdevice.h"
#include "computer.h"
#include "sharebus.h"

class QMenu;
class QAction;
class cableDev;
class device;
class connectDialog;
class devicePort;

// Этот класс содержит нашу сцену и именно здесь
// В итоге будет содержаться вся её функциональность
// Начнем  ...
class myCanvas : public QGraphicsScene
{
Q_OBJECT
public:
    // режимы : нет файла , перемещение , вставка провода , вставка устройства
    enum { noFile = -1 , move = 0 ,cable = 1 , insert = 2 , send = 6};
    enum { width = 2000 , height = 2000 };
    enum sendState { noSendItem = 0 , oneSendItem = 1 };
    // типы устройств : Нет устройства , компьютер , концентратор , коммутатор
    enum { noDev = 0 , busDev = 2 ,compDev = 3 , hubDev = 4 , switchDev = 5 , routerDev = 7 };
    int getMode() { return nowMode; } // Получить текущий режим
    device* addDeviceOnScene(QPointF coor, int myType); // Добавить устройство на сцену
    void deleteConnection(cableDev *cable);
    void hideInsertRect();
    myCanvas(QMenu *context,QObject *parent = 0); // Конструктор
    cableDev* createConnection(device *s,device *e,QString sp,QString ep);
    Q_INVOKABLE void createConnection(int x1,int y1,int x2,int y2,QString strS ,QString strE) {
        device *s = deviceInPoint(QPointF(x1*50+25,y1*50+25));
        device *e = deviceInPoint(QPointF(x2*50+25,y2*50+25));
        createConnection(s,e,strS,strE);
    }
    bool isOpen () const { return myOpen; }
    void setOpen(bool c) { myOpen = c; }
    void setTest(bool c);
    bool test() const { return myTest; }
    int hubSockets() const { return myHubSockets; }
    int switchSockets() const { return mySwitchSockets; }
    int computerSockets() const { return myComputerSockets; }
    int routerSockets() const { return myRouterSockets; }
    int ttlArp() const { return myTtlArp; }
    int ttlMac() const { return myTtlMac; }
    int rip() const { return myRip; }
    bool hubManual() const { return myHubManual; }
    bool switchManual() const { return mySwitchManual; }
    void setComputerSockets(int i) { myComputerSockets = i; }
    void setSwitchSockets(int i) { mySwitchSockets = i; }
    void setHubSockets(int i) { myHubSockets = i; }
    void setRouterSockets(int i) { myRouterSockets = i; }
    void setHubManual( bool m) { myHubManual = m; }
    void setSwitchManual(bool m) { mySwitchManual = m; }
    void setTtlArp(int i) { myTtlArp = i; }
    void setTtlMac(int i) { myTtlMac = i; }
    void setRip(int i) { myRip = i; }
    void motionFrame();
    void whileMotion();
    bool isEnd() const;
    template<class T> T* createDev(short x,short y,int c = 1) {
        T *temp = new T(c);
        temp->setMenu(itemMenu);
        addItem(temp);
        temp->setPos(x*50+25,y*50+25);
        return temp;
    }
    device* oneSelectedDevice();
    device* deviceInPoint(QPointF p);
    int& timerId() { return myTimer; }
    QPointF calibrate(QPointF c);
    ~myCanvas();
signals:
    void uncheck(); // передаем панели что бы сбросила текущее устройство
public slots:
    // Слот получает режим и тип устройства и устанавливает их
    void setMode(int modScene,int curDev);
    void setShowGrid(bool b);
    void removeDevice();
    void newFile();
    void closeFile();
    void saveScene(QString fileName);
    void openScene(QString fileName);
    void createHub(short x,short y,int c) { createDev<hubDevice>(x,y,c); }
    void createRouter(short x,short y,int c) { createDev<routerDevice>(x,y,c); }
    void createComputer(short x,short y,int c) { createDev<computer>(x,y,c); }
    void createSwitch(short x,short y,int c) { createDev<switchDevice>(x,y,c); }
    void createBus(short x,short y,int c) { createDev<shareBus>(x,y,c); }
private:
    bool myOpen;
    bool myTest;
    sendState myState;
    QGraphicsLineItem *line; // Временная линия для рисования
    QGraphicsRectItem *selectRect; // Временный прямоугольник для выделения
    QPointF p2Rect; // Точка начала выделения
    QGraphicsRectItem *insertRect; // Прямоугольныник для вставки
    QGraphicsEllipseItem *sendEllipse; // Кружочек для выделения отправителя и получателя

    QMap<device*,QPointF> coordMap; // Координаты всех перемещаемых устройств

    // All temp transport varios
    int messageSize;
    bool broadcast;
    ipAddress receiverIp;
    device *senderDevice;
    int protocol;

    QMenu *itemMenu; // Меню для устройства
    QList<cableDev* >  connections;
    int nowMode; // Текущий режим
    int nowType; // Текущее устройство
    int prevMode;
    int prevType;

    int myHubSockets;
    int myComputerSockets;
    int mySwitchSockets;
    int myRouterSockets;
    int myTtlArp;
    int myTtlMac;
    int myRip;
    bool myHubManual;
    bool mySwitchManual;
    int myTimer;
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event); // События перемещения
    void mousePressEvent(QGraphicsSceneMouseEvent *event); // нажатия
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event); // и отпускания кнопки мыши
    void keyPressEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent *e);
};

#endif // MYCANVAS_H
