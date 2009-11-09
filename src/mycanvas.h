/****************************************************************************************
** NetEmul - program for simulating computer networks.
** Copyright © 2009 Semenov Pavel and Omilaeva Anastasia
**
** NetEmul is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** NetEmul is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with the NetEmul; if not, write to the Free
** Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
** 02111-1307 USA.
****************************************************************************************/
#ifndef MYCANVAS_H
#define MYCANVAS_H

#include <QtGui/QGraphicsScene>
#include <QtGui/QUndoStack>
#include "textitem.h"

class QMenu;
class QAction;
class cableDev;
class connectDialog;
class devicePort;
class device;
class deviceImpl;
class abstractState;

typedef QList<device*> deviceList;
typedef QList<textItem*> textItemList;
typedef QList<cableDev*> cableList;

static const int UNDO_LIMIT = 7;

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
    // типы устройств : Нет устройства , компьютер , концентратор , коммутатор
    enum { noDev = 0 , busDev = 2 ,compDev = 3 , hubDev = 4 , switchDev = 5 , routerDev = 7 };
    myCanvas(QMenu *context,QObject *parent = 0); // Конструктор
    ~myCanvas();
    device* addDeviceOnScene(QPointF coor, int myType = -1); // Добавить устройство на сцену    
    void hideState();
    cableDev* createConnection(device *s,device *e,QString sp,QString ep);
    textItem* createTextItem(QPointF p, const QString &str = tr("Commentary") );
    bool isOpen () const { return myOpen; }
    void setOpen(bool c) { myOpen = c; }
    void ticTime();
    bool isEnd() const;
    void newFile();
    void closeFile();
    void turnToMove();
    bool isModified() const { return myModified; }
    int animateSpeed() const;
    void setAnimateSpeed(int n);
    device* oneSelectedDevice();
    device* deviceInPoint(QPointF p);
    QPointF calibrate(QPointF c);
    int devicesCount() const { return myDevices.size(); }
    int cablesCount() const { return myConnections.size(); }
    QAction* undoAction(QObject *obj) { return commandStack.createUndoAction(obj); }
    QAction* redoAction(QObject *obj) { return commandStack.createRedoAction(obj); }
    void registerDevice(device *dev);    
    void unregisterDevice(device *dev);
    void registerCable(cableDev *cable);
    void unregisterCable(cableDev *cable);
    void registerText(textItem *t);
    void unregisterText(textItem *t);
    void putItems(QMap<QGraphicsItem*,QPointF> map);
    void calibrateAll(QList<QGraphicsItem*> list);
    bool isDevice(QGraphicsItem *t) const;
signals:
    void uncheck(); //!< Сообщает панели о сбросе текущего устройства
    void fileOpened(); //!< Сообщает главному окно что открыт новый файл
    void fileClosed(); //!< Сообщает главному окну о закрытии файла
    void tictac();
public slots:
    void emulateTime();
    void editorLostFocus(textItem *t);
    void setMode(int modScene,int curDev);
    void setShowGrid(bool b);
    void removeDevice();
    void newScene();
    void closeScene();
    void play();
    void stop() { killTimer(myTimer); myTimer = 0; } // Выключаем таймер
    bool isPlayed() const { return myTimer; }
    void saveScene(QString fileName);
    void openScene(QString fileName);
    void saveSceneXml(QString fileName);
    void openSceneXml(QString fileName);
    deviceImpl* addComputer(int x,int y);
    deviceImpl* addSwitch(int x,int y);
    deviceImpl* addHub(int x,int y);
    deviceImpl* addRouter(int x,int y);
    textItem* addNote(int x, int y);
    QObjectList computerList();
    void addConnection(deviceImpl *s,deviceImpl *e,const QString &sp,const QString &se);
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event); // События перемещения
    void mousePressEvent(QGraphicsSceneMouseEvent *event); // нажатия
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event); // и отпускания кнопки мыши
    void timerEvent(QTimerEvent*);
private:
    QUndoStack commandStack;    
    device* deviceWithImpl(deviceImpl *d);
    bool myOpen;
    bool myModified;
    int lastId;
    abstractState *myState;        

public:
    deviceList devices() { return myDevices; }
private:
    deviceList myDevices; //!< Список всех устройств на сцене.

public:
    textItemList textItems() const { return myTextItems; }
private:
    textItemList myTextItems; //!< Список всех надписей на сцене.

public:
    cableList connections() const { return myConnections; }
private:
    cableList myConnections;

public:
    QMenu* itemMenu() { return myItemMenu; }
    void setItemMenu(QMenu *menu) { myItemMenu = menu; }
private:
    QMenu *myItemMenu; // Меню для устройства

    int nowType; // Текущее устройство

    int myTimer;
// My dear Friends =)
    friend class statisticsScene;
    friend class abstractState;
    friend class moveState;
    friend class insertState;
    friend class cableState;
    friend class textState;
    friend class sendState;
};
//------------------------------------------------------------------
#endif // MYCANVAS_H
