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
#include "cabledev.h"

class QMenu;
class QAction;
class Cable;
class connectDialog;
class DevicePort;
class Device;
class DeviceImpl;
class AbstractState;

typedef QList<Device*> DeviceList;
typedef QList<TextItem*> TextItemList;

static const int UNDO_LIMIT = 7;

/*!
    Класс в котором содержиться вся логика отображения, именно в нем реализована
    вся графическая функциональность программы. Наследник от QGraphicsScene, он получил
    по наследству все самое необходимое, для того что бы мы могли свободно писать логику используя
    уже хорошо зарекомендовавший себя каркас.
*/
class MyCanvas : public QGraphicsScene
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

    enum { OPEN_OK = 0 , READING_FAIL , WRITING_FAIL , OUTDATED_VERSION , STREAM_ERROR };

    MyCanvas(QMenu *context,QObject *parent = 0); // Конструктор
    ~MyCanvas();

    Device* addDeviceOnScene(QPointF coor, int myType = -1); // Добавить устройство на сцену
    Q_INVOKABLE void addDevice(Device *device);
    Q_INVOKABLE void addTextItem(TextItem *item);

    void hideState();
    Cable* createConnection(Device *s,Device *e,QString sp,QString ep);
    TextItem* createTextItem(QPointF p, const QString &str = tr("Commentary") );
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
    Device* oneSelectedDevice();
    Device* deviceInPoint(QPointF p);
    QPointF calibrate(QPointF c);
    int devicesCount() const { return myDevices.size(); }
    int cablesCount() const { return myConnections.size(); }
    QAction* undoAction(QObject *obj) { return commandStack.createUndoAction(obj); }
    QAction* redoAction(QObject *obj) { return commandStack.createRedoAction(obj); }
    void registerDevice(Device *dev);    
    void unregisterDevice(Device *dev);
    void registerCable(Cable *cable);
    void unregisterCable(Cable *cable);
    void registerText(TextItem *t);
    void unregisterText(TextItem *t);
    void putItems(QMap<QGraphicsItem*,QPointF> map);
    void calibrateAll(QList<QGraphicsItem*> list);
    bool isDevice(QGraphicsItem *t) const;

    static QString IOErrorString(int n) {
        static const QString strs[] = { tr("Open complite") ,                                        
                                        tr("Opening file for reading is impossible"),
                                        tr("Opening file for writing is impossible"),
                                        tr("Outdated version of the file, file can't be opened"),
                                        tr("Stream I/O error") };
        return strs[n];
    }

signals:
    void uncheck(); //!< Сообщает панели о сбросе текущего устройства
    void fileOpened(); //!< Сообщает главному окно что открыт новый файл
    void fileClosed(); //!< Сообщает главному окну о закрытии файла
    void tictac();
public slots:
    void emulateTime();
    void editorLostFocus(TextItem *t);
    void setMode(int modScene,int curDev);
    void setShowGrid(bool b);    
    void removeDevice();
    void newScene();
    void closeScene();
    void play();
    void stop() { killTimer(myTimer); myTimer = 0; } // Выключаем таймер
    bool isPlayed() const { return myTimer; }
    int saveScene(const QString &fileName);
    int openScene(const QString &fileName);
    int saveSceneXml(const QString &fileName);
    int openSceneXml(const QString &fileName);
    DeviceImpl* addComputer(int x,int y);
    DeviceImpl* addSwitch(int x,int y);
    DeviceImpl* addHub(int x,int y);
    DeviceImpl* addRouter(int x,int y);
    TextItem* addNote(int x, int y);
    QObjectList computerList();
    void addConnection(DeviceImpl *s,DeviceImpl *e,const QString &sp,const QString &se);

    void addCableDev(Cable *cable);

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event); // События перемещения
    void mousePressEvent(QGraphicsSceneMouseEvent *event); // нажатия
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event); // и отпускания кнопки мыши
    void timerEvent(QTimerEvent*);
private:
    QUndoStack commandStack;    
    Device* deviceWithImpl(DeviceImpl *d);
    bool myOpen;
    bool myModified;    
    AbstractState *myState;

public:
    DeviceList devices() const { return myDevices; }
private:
    DeviceList myDevices; //!< Список всех устройств на сцене.

public:
    TextItemList textItems() const { return myTextItems; }
private:
    TextItemList myTextItems; //!< Список всех надписей на сцене.

public:
    CableList connections() const { return myConnections; }
private:
    CableList myConnections;

public:
    QMenu* itemMenu() { return myItemMenu; }
    void setItemMenu(QMenu *menu) { myItemMenu = menu; }
private:
    QMenu *myItemMenu; // Меню для устройства

    int nowType; // Текущее устройство

    int myTimer;
// My dear Friends =)
    friend class StatisticsScene;
    friend class AbstractState;
    friend class MoveState;
    friend class InsertState;
    friend class CableState;
    friend class TextState;
    friend class SendState;
};
//------------------------------------------------------------------

#endif // MYCANVAS_H
