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
#include "mycanvas.h"
#include "device.h"
#include "cabledev.h"
#include "appsetting.h"
#include "abstractstate.h"

/*!
  Конструктор проводит начальную инициализацию сцены.
  @param context - Контекстное menu устройств.
  @param parent - Объект родитель.
*/
myCanvas::myCanvas(QMenu *context, QObject *parent) : QGraphicsScene(parent)
{
    itemMenu = context; // меню из аргумента                    
    myTimer = 0;       
    myOpen = false;
    myModified = false;
    myState = abstractState::initialize(this);
}
//------------------------------------------------------------------
/*!
 * Деструктор пока не выполняет ни каких действий.
*/ 
myCanvas::~myCanvas()
{
    clear();
    myDevices.clear();
    connections.clear();
    myTextItems.clear();
}
//------------------------------------------------------------------
/*!
 * Событие перемещения мыши.
 * @param event - переменная события
*/ 
void myCanvas::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    myState->mouseMove(event);
}
//----------------------------------------------------------------------
/*!
  Событие нажатия мыши.
  @param event - переменная события.
*/
void myCanvas::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if ( event->button() != Qt::LeftButton ) return;
    myState->mousePress(event);
}
//-----------------------------------------------------------------------
void myCanvas::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
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
cableDev* myCanvas::createConnection(device *s , device *e , QString sp,QString ep)
{
    if ( !s || !e ) return 0; // Если хотя бы одного устройства нет, то выходим.
    cableDev *cable = new cableDev(s, e, sp , ep ); // Создаем между ними кабель
    s->update();
    e->update();
    addItem(cable); // И добавляем его на сцену =)
    connections << cable;
    cable->updatePosition(); // Обновляем его положение
    myModified = true;
    return cable;
}
//-------------------------------------------------------------------------

device* myCanvas::addDeviceOnScene(QPointF coor, int myType)
{
    device *t = new device(myType);
    t->setPos( calibrate(coor) );
    t->setMenu(itemMenu);
    addItem(t);
    myDevices << t;
    return t;
}
/*!
  Функция удаляет со сцены выделенные устройства и провода.
*/
void myCanvas::removeDevice()
{
    myModified = true;
    QList<QGraphicsItem*> l = selectedItems(); // Получаем список выделенных элементов.
    foreach (QGraphicsItem *item, l ) {
        if ( isDevice(item) ) { // Если не кабель
            device *t = qgraphicsitem_cast<device*>(item);
            QList<cableDev*> lostCables = t->cables(); // Удаляем все кабеля у этого устройства
            foreach ( cableDev* i , lostCables) {
                removeItem(i);
                deleteConnection(i);
            }
            myDevices.removeOne(t);
        }
        else if ( item->type() == cableDev::Type ) {
            deleteConnection(qgraphicsitem_cast<cableDev*>(item)); // Иначе удаем кабель
        }
        else if ( item->type() == textItem::Type )
            myTextItems.removeOne(qgraphicsitem_cast<textItem*>(item));
        removeItem(item); // Удаляем этот элемент со сцены
    }
}
//------------------------------------------------------
/*!
  Открывает новый файл, проводит подготовку внешнего вида сцены.
*/
void myCanvas::newFile()
{
    if ( myOpen ) return;
    lastId = 0;
    setBackgroundBrush(QBrush(QPixmap(":im/images/back.png")));
    setSceneRect(0,0,myCanvas::width,myCanvas::height);
    myState->goMove();
    myOpen = true;
    play();
}
//-------------------------------------------------
/*!
  Закрывает файл, очищает сцену, делает фон серым, удаляет все соединения.
*/
void myCanvas::closeFile()
{   
    clear();
    myDevices.clear();
    setBackgroundBrush(QBrush(Qt::lightGray));
    setSceneRect(0,0,1,1);
    connections.clear();
    myTextItems.clear();
    if ( myTimer ) stop();
    myOpen = false;
    myModified = false;
    myState->goEmpty();
}
//---------------------------------------------------
void myCanvas::deleteConnection(cableDev *cable)
{
    cable->deleteConnect();
    connections.removeOne(cable);
}

void myCanvas::setMode(int modScene,int curDev)
{
    myState->goTo(modScene);
    nowType = curDev;    
    setSelectionArea( QPainterPath() );
}

void myCanvas::hideState()
{
    myState->hideState();
}
/*!
  Загружает сцену из файла.
  @param fileName - имя файла из которого осуществляется загрузка.
*/
void myCanvas::openScene(QString fileName)
{
    newFile();
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Невозможно открыть файл для чтения";
        return;
    }
    QDataStream s(&file);
    s.setVersion(QDataStream::Qt_4_3);
    QPointF p;
    QString str;
    s >> str;
    if ( str != QCoreApplication::applicationVersion() ) {
        QMessageBox::critical(NULL,tr("Error"),tr("The outdated version of the file, file can't be opened"),
                              QMessageBox::Ok , QMessageBox::Ok );
        emit fileClosed();
        return;
    }
    QApplication::changeOverrideCursor(Qt::WaitCursor);
    device *item;
    int n,i;
    s >> n;
    for ( i = 0 ; i < n ; i++ ) {
        item = new device(s);
        item->setMenu(itemMenu);
        addItem(item);
        myDevices << item;
    }
    s >> n;
    for ( i = 0 ; i < n ; i++ ) {
        s >> p;
        device *start = deviceInPoint(p);
        s >> p;
        device *end = deviceInPoint(p);
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
    qDebug() << QString("Scene was been open from %1").arg(fileName) ;
    myModified = false;
}
//-----------------------------------------------------------------------
/*!
  Сохраняет сцену в файл.
  @param fileName - имя файла в который осуществляется сохранение.
*/
void myCanvas::saveScene(QString fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Невозможно открыть файл для записи";
        return;
    }
    QApplication::changeOverrideCursor(Qt::WaitCursor);
    QDataStream s(&file);
    s.setVersion(QDataStream::Qt_4_3);
    s << QCoreApplication::applicationVersion();
    s << myDevices.size();
    foreach(device *i, myDevices)
        s << *i;
    s << connections.count();
    foreach (cableDev *i, connections) {
        s << i->line().p1() << i->line().p2();
        s << i->startSocketName() << i->endSocketName() ;
    }
    s << myTextItems.count();
    foreach ( textItem *i, myTextItems ) {
        s << i->pos();
        s << i->toPlainText();
    }
    if ( s.status() != QDataStream::Ok ) qDebug() << "PPC";
    file.close();
    QApplication::restoreOverrideCursor();
    qDebug() << QString("Scene was been saved in %1").arg(fileName) ;
    myModified = false;
}
//-------------------------------------------------------------------------
/*!
  Событие таймера сцены выполняет очень много функций, так много что необходимо
  разнести из по разным потокам, но не сейчас. Происходят следующие действия:
  обновление arp,mac, rip таблиц и сдвиг пакетов на кабелях.
*/
void myCanvas::timerEvent(QTimerEvent*)
{
    ticTime();
}
//--------------------------------------------------------------
void myCanvas::ticTime()
{
    static int n = 9;
    foreach ( cableDev *t , connections)
        if ( t->isBusy() ) t->motion();
    n--;
    foreach ( device *i, myDevices ) {
        i->deciSecondTimerEvent();
        if ( !n ) i->secondTimerEvent();
    }
    if ( !n ) n = 9;
}

void myCanvas::emulateTime()
{
   while ( !isEnd() ) ticTime();
}

bool myCanvas::isEnd() const
{
    foreach ( cableDev *t , connections ) {
        if ( t->isBusy() ) return false;
    }
    foreach ( device *i , myDevices )
        if ( i->isBusy() ) {
            return false;
        }
    return true;
}

device* myCanvas::oneSelectedDevice()
{
    if ( selectedItems().count() == 1 && isDevice( selectedItems().first() ) )
        return qgraphicsitem_cast<device*>(selectedItems().first());
    return NULL;
}

device* myCanvas::deviceInPoint(QPointF p)
{
    foreach ( QGraphicsItem *i , items(p) )
        if ( isDevice(i) ) return qgraphicsitem_cast<device*>(i);
    return 0;
}


void myCanvas::setShowGrid(bool b)
{
    if (b) setBackgroundBrush(QBrush(QPixmap(":im/images/back.png")));
    else setBackgroundBrush(QBrush(Qt::white));
}
/*!
  Выравнивает координаты точки по сетке.
  @param с - точка подлещашая выравниванию.
  @return - выровненая точка.
*/
QPointF myCanvas::calibrate(QPointF c)
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
void myCanvas::editorLostFocus(textItem *t)
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
textItem* myCanvas::createTextItem(QPointF p , const QString &str /*=tr("Комментарий")*/)
{
    textItem *t = new textItem(p);
    t->setPlainText(str);
    t->adjustSize();
    connect(t,SIGNAL(lostFocus(textItem*)),SLOT(editorLostFocus(textItem*)));
    addItem(t);
    myTextItems << t;
    return t;
}
//--------------------------------------------------------------------
/*!
  Проверяет устройство ли данный объект или нет.
  @return true если устройство, false в противном случае.
*/
bool myCanvas::isDevice(QGraphicsItem *t) const
{
    if ( t->type() == device::Type ) return true;
    return false;
}
//------------------------------------------------------------------------

deviceImpl* myCanvas::addComputer(int x,int y)
{
    device *t = addDeviceOnScene(QPointF(x*50+25,y*50+25) , compDev);
    return t->contentDevice();
}
deviceImpl* myCanvas::addRouter(int x,int y)
{
    device *t = addDeviceOnScene(QPointF(x*50+25,y*50+25) , routerDev);
    return t->contentDevice();
}
deviceImpl* myCanvas::addHub(int x,int y)
{
    device *t = addDeviceOnScene(QPointF(x*50+25,y*50+25) , hubDev);
    return t->contentDevice();
}

deviceImpl* myCanvas::addSwitch(int x,int y)
{
    device *t = addDeviceOnScene(QPointF(x*50+25,y*50+25) , switchDev);
    return t->contentDevice();
}

textItem* myCanvas::addNote(int x, int y)
{
    QPointF p( x*50 , y*50 );
    return createTextItem(p);
}

void myCanvas::addConnection(deviceImpl *s,deviceImpl *e, const QString &sp,const QString &se)
{
    device *st = deviceWithImpl(s);
    device *et = deviceWithImpl(e);
    createConnection(st,et,sp,se);
}

device* myCanvas::deviceWithImpl(deviceImpl *d)
{
    foreach ( device *i , myDevices )
        if ( i->contentDevice() == d ) return i;
    return 0;
}

int myCanvas::animateSpeed() const
{
    return appSetting::animateSpeed();
}

void myCanvas::setAnimateSpeed(int n)
{
    if (!myTimer ) return;
    killTimer(myTimer);
    appSetting::setAnimateSpeed(n);
    myTimer = startTimer(appSetting::realAnimateSpeed());
}

void myCanvas::play()
{
    myTimer = startTimer( appSetting::realAnimateSpeed() );
}

QObjectList myCanvas::computerList()
{
    QObjectList temp;
    foreach ( device *i , myDevices )
        if ( i->isCanSend() ) temp << i->contentDevice();
    return temp;
}

void myCanvas::closeScene()
{
    emit fileClosed();
}

void myCanvas::newScene()
{
    emit fileOpened();
}





