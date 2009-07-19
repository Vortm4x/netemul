#ifndef MYCANVAS_H
#define MYCANVAS_H

#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include "macaddress.h"
#include "ipaddress.h"
#include "interface.h"
#include "textitem.h"

class QMenu;
class QAction;
class cableDev;
class connectDialog;
class devicePort;
class textItem;
/*!
    Класс в котором содержиться вся логика отображения, именно в нем реализована
    вся графическая функциональность программы. Наследник от QGraphicsScene, он получил
    по наследству все самое необходимое, для того что бы мы могли свободно писать логику используя
    уже хорошо зарекомендовавший себя каркас.
*/
class myCanvas : public QGraphicsScene
{
    Q_OBJECT
    Q_PROPERTY(bool open READ isOpen WRITE setOpen)
public:
    // режимы : нет файла , перемещение , вставка провода , вставка устройства
    enum { noFile = -1 , move = 0 , cable = 1 , insert = 2 , send = 6 , text = 8};
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
    textItem* createTextItem();
    bool isOpen () const { return myOpen; }
    void setOpen(bool c) { myOpen = c; }
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
    void setShapeView(QAbstractGraphicsShapeItem *i , QPen p ,QBrush b);
    bool isEnd() const;
    bool isDevice(QGraphicsItem *t) const;
    device* oneSelectedDevice();
    device* deviceInPoint(QPointF p);
    QPointF calibrate(QPointF c);
    ~myCanvas();
signals:
    void uncheck(); //!< Сообщает панели о сбросе текущего устройства
    void fileOpened(); //!< Сообщает главному окно что открыт новый файл
    void fileClosed(); //!< Сообщает главному окну о закрытии файла
public slots:
    void editorLostFocus(textItem *t);
    void setMode(int modScene,int curDev);
    void setShowGrid(bool b);
    void removeDevice();
    void newFile();
    void closeFile();
    void play() { myTimer = startTimer(100); } // Включаем основной таймер
    void stop() { killTimer(myTimer); myTimer = 0; } // Выключаем таймер
    bool isPlayed() const { return myTimer; }
    void saveScene(QString fileName);
    void openScene(QString fileName);
private:
    bool myOpen;
    int lastId;
    sendState myState;
    QGraphicsLineItem *line; // Временная линия для рисования
    QGraphicsRectItem *selectRect; // Временный прямоугольник для выделения
    QPointF p2Rect; // Точка начала выделения
    QGraphicsRectItem *insertRect; // Прямоугольныник для вставки
    QGraphicsEllipseItem *sendEllipse; // Кружочек для выделения отправителя и получателя
    textItem *textRect; // Временный указатель на текст.

    QMap<QGraphicsItem*,QPointF> coordMap; //!< Соответствия перемещаемых в данный момент устройств и их координат
    QList<device*> myDevices; //!< Список всех устройств на сцене.
    QList<textItem*> myTextItems; //!< Список всех надписей на сцене.

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
    void timerEvent(QTimerEvent *e);
};
//------------------------------------------------------------------
/*!
  Проверяет устройство ли данный объект или нет.
  @return true если устройство, false в противном случае.
*/
inline bool myCanvas::isDevice(QGraphicsItem *t) const
{
    if ( t->type() != cableDev::Type && t->type() != textItem::Type ) return true;
    return false;
}
//------------------------------------------------------------------------
#endif // MYCANVAS_H
