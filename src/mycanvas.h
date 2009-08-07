#ifndef MYCANVAS_H
#define MYCANVAS_H

#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include "textitem.h"

class QMenu;
class QAction;
class cableDev;
class connectDialog;
class devicePort;
class textItem;
class device;
class deviceImpl;
class insertRect;
class sendEllipse;
class selectRect;
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
    Q_PROPERTY(int animateSpeed READ animateSpeed WRITE setAnimateSpeed)
public:
    // режимы : нет файла , перемещение , вставка провода , вставка устройства
    enum { noFile = -1 , move = 0 , cable = 1 , insert = 2 , send = 6 , text = 8};
    enum { width = 2000 , height = 2000 };
    enum sendState { noSendItem = 0 , oneSendItem = 1 };
    // типы устройств : Нет устройства , компьютер , концентратор , коммутатор
    enum { noDev = 0 , busDev = 2 ,compDev = 3 , hubDev = 4 , switchDev = 5 , routerDev = 7 };
   // int getMode() { return nowMode; } // Получить текущий режим
    device* addDeviceOnScene(QPointF coor, int myType); // Добавить устройство на сцену
    void deleteConnection(cableDev *cable);
    void hideInsertRect();
    myCanvas(QMenu *context,QObject *parent = 0); // Конструктор
    cableDev* createConnection(device *s,device *e,QString sp,QString ep);
    textItem* createTextItem(QPointF p, const QString &str = tr("Commentary") );
    bool isOpen () const { return myOpen; }
    void setOpen(bool c) { myOpen = c; }
    void ticTime();
    bool isEnd() const;
    int animateSpeed() const;
    void setAnimateSpeed(int n);
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
    void emulateTime();
    void editorLostFocus(textItem *t);
    void setMode(int modScene,int curDev);
    void setShowGrid(bool b);
    void removeDevice();
    void newFile();
    void closeFile();
    void play();
    void stop() { killTimer(myTimer); myTimer = 0; } // Выключаем таймер
    bool isPlayed() const { return myTimer; }
    void saveScene(QString fileName);
    void openScene(QString fileName);
    deviceImpl* addComputer(int x,int y);
    deviceImpl* addSwitch(int x,int y);
    deviceImpl* addHub(int x,int y);
    deviceImpl* addRouter(int x,int y);
    void addConnection(deviceImpl *s,deviceImpl *e,const QString &sp,const QString &se);
private:
    device* deviceWithImpl(deviceImpl *d);
    bool myOpen;
    int lastId;
    sendState myState;
    QGraphicsLineItem *line; // Временная линия для рисования
    selectRect *SelectRect; // Временный прямоугольник для выделения
    QPointF p2Rect; // Точка начала выделения
    insertRect *InsertRect; // Прямоугольныник для вставки
    sendEllipse *SendEllipse; // Кружочек для выделения отправителя и получателя

    QMap<QGraphicsItem*,QPointF> coordMap; //!< Соответствия перемещаемых в данный момент устройств и их координат
    QList<device*> myDevices; //!< Список всех устройств на сцене.
    QList<textItem*> myTextItems; //!< Список всех надписей на сцене.

    // All temp transport varios
    int messageSize;
    bool broadcast;
    QString receiverIp;
    device *senderDevice;
    int protocol;

    QMenu *itemMenu; // Меню для устройства
    QList<cableDev* >  connections;
    int nowMode; // Текущий режим
    int nowType; // Текущее устройство
    int prevMode;
    int prevType;

    int myTimer;
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event); // События перемещения
    void mousePressEvent(QGraphicsSceneMouseEvent *event); // нажатия
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event); // и отпускания кнопки мыши
    void timerEvent(QTimerEvent*);
};
//------------------------------------------------------------------
#endif // MYCANVAS_H
