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
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QKeyEvent>
#include <QtGui/QGraphicsView>
#include <QtGui/QMenu>
#include <QAction>
#include <QPainter>
#include <QtDebug>
#include <QtCore/QFile>
#include <QtCore/QDataStream>
#include <QtGui/QMessageBox>
#include <QtGui/QTextCursor>
#include <QtGui/QApplication>
#include <QtXml/QXmlSimpleReader>

#include "mycanvas.h"
#include "device.h"
#include "cabledev.h"
#include "appsetting.h"
#include "abstractstate.h"
#include "deletecommand.h"
#include "addcablecommand.h"
#include "scenexmlwriter.h"
#include "scenexmlreader.h"

/*!
  Конструктор проводит начальную инициализацию сцены.
  @param context - Контекстное menu устройств.
  @param parent - Объект родитель.
*/
MyCanvas::MyCanvas(QMenu *context, QObject *parent) : QGraphicsScene(parent)
{
    myItemMenu = context; // меню из аргумента
    myTimer = 0;       
    myOpen = false;
    myModified = false;
    myState = abstractState::initialize(this);
    commandStack.setUndoLimit(UNDO_LIMIT);

    // WHAT THE FUCK???
    TextItem *i = createTextItem(QPointF(0,0),"123");
    unregisterText(i);
    delete i;
}
//------------------------------------------------------------------
/*!
 * Деструктор пока не выполняет ни каких действий.
*/ 
MyCanvas::~MyCanvas()
{
    clear();
    myDevices.clear();
    myConnections.clear();
    myTextItems.clear();
}
//------------------------------------------------------------------
/*!
 * Событие перемещения мыши.
 * @param event - переменная события
*/ 
void MyCanvas::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{    
    myState->mouseMove(event);
}
//----------------------------------------------------------------------
/*!
  Событие нажатия мыши.
  @param event - переменная события.
*/
void MyCanvas::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if ( event->button() != Qt::LeftButton ) return;
    myState->mousePress(event);
}
//-----------------------------------------------------------------------
void MyCanvas::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if ( event->button() != Qt::LeftButton ) return;
    myState->mouseRelease(event);
}
/*!
  Создает соединение между устройствами.
  @param s - Указатель на первое устройство.
  @param e - Указатель на второе устройство.
  @param sp - Имя порта первого устройства.
  @param ep - Имя порта второго устройства.
  @return указатель на кабель соединяющий устройства.
*/
Cable* MyCanvas::createConnection(Device *s , Device *e , QString sp,QString ep)
{
    if ( !s || !e ) {
        qDebug("Panika Panika!");
        return 0; // Если хотя бы одного устройства нет, то выходим.
    }
    Cable *cable = new Cable(); // Создаем между ними кабель
    DevicePort *p1 = s->findPortByName(sp);
    DevicePort *p2 = e->findPortByName(ep);
    if ( !p1 || !p2 ) {
//        qDebug("Device s : %d - %s , e : %d - %s , DevicePort: p1 : %d , p2 : %d",s,qPrintable(sp),e,
//               qPrintable(ep),p1,p2);
        return 0; // Если хотя бы одного устройства нет, то выходим.
    }
    cable->setStartPort( p1 );
    cable->setEndPort( p2 );
    cable->setStartItem(s);
    cable->setEndItem(e);
    addCableCommand *com = new addCableCommand(this, cable);
    commandStack.push(com);
    myModified = true;
    cable->updatePosition();
    return cable;
}
//-------------------------------------------------------------------------

void MyCanvas::addCableDev(Cable *cable)
{
    registerCable(cable);
}

Device* MyCanvas::addDeviceOnScene(QPointF coor, int myType /* = -1 */)
{
    if ( myType == -1 ) myType = nowType;
    Device *t = new Device(myType);
    t->setPos( calibrate(coor) );    
    addDevice(t);
    return t;
}

void MyCanvas::addDevice(Device *device)
{
    device->setMenu(myItemMenu);
    registerDevice(device);
}

/*!
  Функция удаляет со сцены выделенные устройства и провода.
*/
void MyCanvas::removeDevice()
{
    myModified = true;
    QList<QGraphicsItem*> list = selectedItems(); // Получаем список выделенных элементов.
    deleteCommand *com = new deleteCommand(this,list);
    commandStack.push(com);
}
//------------------------------------------------------
/*!
  Открывает новый файл, проводит подготовку внешнего вида сцены.
*/
void MyCanvas::newFile()
{
    if ( myOpen ) return;    
    setBackgroundBrush(QBrush(QPixmap(":im/images/back.png")));
    setSceneRect(0,0,MyCanvas::width,MyCanvas::height);
    myState->goMove();
    myOpen = true;    
    play();
}
//-------------------------------------------------
/*!
  Закрывает файл, очищает сцену, делает фон серым, удаляет все соединения.
*/
void MyCanvas::closeFile()
{   
    myState->goEmpty();
    commandStack.clear();
    clear();
    myDevices.clear();
    setBackgroundBrush(QBrush(Qt::lightGray));
    setSceneRect(0,0,1,1);
    myConnections.clear();
    myTextItems.clear();
    if ( myTimer ) stop();
    myOpen = false;
    myModified = false;
}
//---------------------------------------------------
void MyCanvas::setMode(int modScene,int curDev)
{
    myState->goTo(modScene);
    nowType = curDev;    
    setSelectionArea( QPainterPath() );
}

void MyCanvas::hideState()
{
    myState->hideState();
}
/*!
  Загружает сцену из файла.
  @param fileName - имя файла из которого осуществляется загрузка.
*/
void MyCanvas::openScene(QString fileName)
{
    newFile();
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << tr("Opening file for reading is impossible");
        return;
    }
    QDataStream s(&file);
    s.setVersion(QDataStream::Qt_4_3);
    QPointF p;
    QString str;
    s >> str;
    if ( str != QCoreApplication::applicationVersion() ) {
        QMessageBox::critical(0,tr("Error"),tr("Outdated version of the file, file can't be opened"),
                              QMessageBox::Ok , QMessageBox::Ok );
        emit fileClosed();
        return;
    }
    QApplication::changeOverrideCursor(Qt::WaitCursor);
    Device *item;
    int n,i;
    s >> n;
    for ( i = 0 ; i < n ; i++ ) {
        item = new Device(s);
        item->setMenu(myItemMenu);
        addItem(item);
        myDevices << item;
    }
    s >> n;
    for ( i = 0 ; i < n ; i++ ) {
        s >> p;
        Device *start = deviceInPoint(p);
        s >> p;
        Device *end = deviceInPoint(p);
        s >> str;
        QString startP = str;
        s >> str;
        QString endP = str;
        createConnection( start , end , startP , endP );
    }
    s >> n;
    for ( i = 0 ; i < n ; i++ ) {
        s >> p; s >> str;
        createTextItem(p,str);
    }
    if ( s.status() != QDataStream::Ok ) qDebug() << "PPC";
    file.close();
    QApplication::restoreOverrideCursor();
    emit fileOpened();
    qDebug() << tr("Scene opened from %1").arg(fileName) ;
    myModified = false;
}
//-----------------------------------------------------------------------

void MyCanvas::openSceneXml(QString fileName)
{
    newFile();
    stop();
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << tr("Opening file for reading is impossible");
        return;
    }
    QApplication::changeOverrideCursor(Qt::WaitCursor);

    SceneXmlReader handler(this);
    QXmlSimpleReader reader;
    reader.setContentHandler(&handler);
    QXmlInputSource source(&file);

    if ( reader.parse(&source) ) {
        emit fileOpened();
        qDebug() << tr("Scene opened from %1").arg(fileName) ;
    } else {
        qDebug("CДелай Ну ХОТЬ ЧТО_НИБУДЬ!!!!!!!!!");
    }

    file.close();
    play();
    QApplication::restoreOverrideCursor();
    myModified = false;
}

/*!
  Сохраняет сцену в файл.
  @param fileName - имя файла в который осуществляется сохранение.
*/
void MyCanvas::saveScene(QString fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << tr("Opening file for writing is impossible %1").arg(fileName);
        return;
    }
    QApplication::changeOverrideCursor(Qt::WaitCursor);
    QDataStream s(&file);
    s.setVersion(QDataStream::Qt_4_3);
    s << QCoreApplication::applicationVersion();
    s << myDevices.size();
    foreach(Device *i, myDevices)
        s << *i;
    s << myConnections.count();
    foreach (Cable *i, myConnections) {
        QPointF p1 = i->startPos();
        QPointF p2 = i->endPos();
        Device *d1 = deviceInPoint(p1);
        Device *d2 = deviceInPoint(p2);
        s << p1 << p2;
        s << d1->socketName(i) << d2->socketName(i);
    }
    s << myTextItems.count();
    foreach ( TextItem *i, myTextItems ) {
        s << i->pos();
        s << i->toPlainText();
    }
    if ( s.status() != QDataStream::Ok ) qDebug() << "PPC";
    file.close();
    QApplication::restoreOverrideCursor();
    qDebug() << tr("Scene saved in %1").arg(fileName) ;
    myModified = false;
}

void MyCanvas::saveSceneXml(QString fileName)
{
    QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << tr("Opening file for writing is impossible %1").arg(fileName);
        return;
    }
    QApplication::changeOverrideCursor(Qt::WaitCursor);
    sceneXmlWriter s(this);
    s.writeScene(&file);
    file.close();
    QApplication::restoreOverrideCursor();
    qDebug() << tr("Scene saved in %1").arg(fileName) ;
    myModified = false;
}
//-------------------------------------------------------------------------
/*!
  Событие таймера сцены выполняет очень много функций, так много что необходимо
  разнести из по разным потокам, но не сейчас. Происходят следующие действия:
  обновление arp,mac, rip таблиц и сдвиг пакетов на кабелях.
*/
void MyCanvas::timerEvent(QTimerEvent*)
{
    ticTime();
}
//--------------------------------------------------------------
void MyCanvas::ticTime()
{
    static int n = 9;
    foreach ( Cable *t , myConnections)
        if ( t->isBusy() ) t->motion();
    n--;
    foreach ( Device *i, myDevices ) {
        i->deciSecondTimerEvent();
        if ( !n ) i->secondTimerEvent();
    }
    if ( !n ) {
        n = 9;
        emit tictac();
    }
}

void MyCanvas::emulateTime()
{
   while ( !isEnd() ) ticTime();
}

bool MyCanvas::isEnd() const
{
    foreach ( Cable *t , myConnections ) {
        if ( t->isBusy() ) return false;
    }
    foreach ( Device *i , myDevices )
        if ( i->isBusy() ) {
            return false;
        }
    return true;
}

Device* MyCanvas::oneSelectedDevice()
{
    if ( selectedItems().count() == 1 && isDevice( selectedItems().first() ) )
        return qgraphicsitem_cast<Device*>(selectedItems().first());
    return 0;
}

Device* MyCanvas::deviceInPoint(QPointF p)
{
    foreach ( QGraphicsItem *i , items(p) )
        if ( isDevice(i) ) return qgraphicsitem_cast<Device*>(i);
    return 0;
}

void MyCanvas::setShowGrid(bool b)
{
    if (b) setBackgroundBrush(QBrush(QPixmap(":im/images/back.png")));
    else setBackgroundBrush(QBrush(Qt::white));
}
/*!
  Выравнивает координаты точки по сетке.
  @param с - точка подлещашая выравниванию.
  @return - выровненая точка.
*/
QPointF MyCanvas::calibrate(QPointF c)
{
    c.setX( (qRound(c.x()) / 50)*50+25 );
    c.setY( (qRound(c.y()) / 50)*50+25 );
    return c;
}
//--------------------------------------------------------------------
/*!
  Происходит при потере текстовой надписью фокуса, служит для уничтожения надписи,
  в случае если она пуста.
  @param t - указатель на надпись.
*/
void MyCanvas::editorLostFocus(TextItem *t)
{
     QTextCursor cursor = t->textCursor();
     cursor.clearSelection();
     t->setTextCursor(cursor);
     t->adjustSize();
     if ( t->toPlainText().isEmpty()) {
         removeItem(t);
         myTextItems.removeOne(t);
         t->deleteLater();
     }
}
//--------------------------------------------------------------------
/*!
  Создает на сцене новый комментарий.
  @return указатель на созданный комментарий.
*/
TextItem* MyCanvas::createTextItem(QPointF p , const QString &str /*=tr("Комментарий")*/)
{
    TextItem *t = new TextItem(p);
    t->setPlainText(str);
    t->adjustSize();
    connect(t,SIGNAL(lostFocus(TextItem*)),SLOT(editorLostFocus(TextItem*)));
    addItem(t);
    myTextItems << t;
    return t;
}
//--------------------------------------------------------------------
/*!
  Проверяет устройство ли данный объект или нет.
  @return true если устройство, false в противном случае.
*/
bool MyCanvas::isDevice(QGraphicsItem *t) const
{
    if ( t->type() == Device::Type ) return true;
    return false;
}
//------------------------------------------------------------------------

DeviceImpl* MyCanvas::addComputer(int x,int y)
{
    Device *t = addDeviceOnScene(QPointF(x*50+25,y*50+25) , compDev);
    return t->contentDevice();
}
DeviceImpl* MyCanvas::addRouter(int x,int y)
{
    Device *t = addDeviceOnScene(QPointF(x*50+25,y*50+25) , routerDev);
    return t->contentDevice();
}
DeviceImpl* MyCanvas::addHub(int x,int y)
{
    Device *t = addDeviceOnScene(QPointF(x*50+25,y*50+25) , hubDev);
    return t->contentDevice();
}

DeviceImpl* MyCanvas::addSwitch(int x,int y)
{
    Device *t = addDeviceOnScene(QPointF(x*50+25,y*50+25) , switchDev);
    return t->contentDevice();
}

TextItem* MyCanvas::addNote(int x, int y)
{
    QPointF p( x*50 , y*50 );
    return createTextItem(p);
}

void MyCanvas::addTextItem(TextItem *item)
{
    item->adjustSize();
    connect(item,SIGNAL(lostFocus(TextItem*)),SLOT(editorLostFocus(TextItem*)));
    addItem(item);
    myTextItems << item;
}

void MyCanvas::addConnection(DeviceImpl *s,DeviceImpl *e, const QString &sp,const QString &se)
{
    Device *st = deviceWithImpl(s);
    Device *et = deviceWithImpl(e);
    createConnection(st,et,sp,se);
}

Device* MyCanvas::deviceWithImpl(DeviceImpl *d)
{
    foreach ( Device *i , myDevices )
        if ( i->contentDevice() == d ) return i;
    return 0;
}

int MyCanvas::animateSpeed() const
{
    return appSetting::animateSpeed();
}

void MyCanvas::setAnimateSpeed(int n)
{
    if (!myTimer ) return;
    killTimer(myTimer);
    appSetting::setAnimateSpeed(n);
    myTimer = startTimer(appSetting::realAnimateSpeed());
}

void MyCanvas::play()
{
    myTimer = startTimer( appSetting::realAnimateSpeed() );
}

QObjectList MyCanvas::computerList()
{
    QObjectList temp;
    foreach ( Device *i , myDevices )
        if ( i->isCanSend() ) temp << i->contentDevice();
    return temp;
}

void MyCanvas::closeScene()
{
    emit fileClosed();
}

void MyCanvas::newScene()
{
    emit fileOpened();
}

void MyCanvas::turnToMove()
{
    myState->goMove();
    emit uncheck();
}

void MyCanvas::putItems(QMap<QGraphicsItem*,QPointF> map)
{
    QMapIterator<QGraphicsItem*,QPointF> i(map);
    i.toFront();
    while ( i.hasNext() ) {
        i.next();
        QGraphicsItem *curDevice = i.key();
        QPointF curPoint = i.value();

        curDevice->setPos( curPoint );

        if ( isDevice( curDevice ) ) {
            Device *d = qgraphicsitem_cast<Device*>(curDevice);
            d->updateCables();
        }
    }
    calibrateAll( map.keys() );
    myModified = true;
}

void MyCanvas::calibrateAll(QList<QGraphicsItem*> list)
{
    foreach ( QGraphicsItem *i , list )
        if ( i->type() != TextItem::Type ) i->setPos( calibrate( i->pos() ) );
}

void MyCanvas::registerDevice(Device *dev)
{
    addItem(dev);
    myDevices << dev;
}

void MyCanvas::unregisterDevice(Device *dev)
{
    removeItem(dev);
    myDevices.removeOne(dev);
}

void MyCanvas::registerCable(Cable *cable)
{
    addItem(cable); // И добавляем его на сцену =)
    myConnections << cable;
}

void MyCanvas::unregisterCable(Cable *cable)
{
    removeItem(cable);
    myConnections.removeOne(cable);
}

void MyCanvas::registerText(TextItem *t)
{
    addItem(t);
    myTextItems << t;
}

void MyCanvas::unregisterText(TextItem *t)
{
    removeItem(t);
    myTextItems.removeOne(t);
}
