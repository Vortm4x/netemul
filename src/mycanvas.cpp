#include "mycanvas.h"
#include "connectdialog.h"
#include "device.h"
#include "cabledev.h"
#include "senddialog.h"
#include "appsetting.h"
#include "insertrect.h"
#include "sendellipse.h"
#include "selectrect.h"
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QMenu>
#include <QAction>
#include <QPainter>
#include <QtDebug>
#include <QFile>
#include <QDataStream>
#include <QMessageBox>
#include <QTextCursor>

/*!
  Конструктор проводит начальную инициализацию сцены.
  @param context - Контекстное устройств.
  @param parent - Объект родитель.
*/
myCanvas::myCanvas(QMenu *context, QObject *parent) : QGraphicsScene(parent)
{
    nowMode = noFile; // Сначала файла нет
    itemMenu = context; // меню из аргумента
    line = 0; // Провода нет
    SelectRect = 0; // Выделения нет
    p2Rect = QPoint();
    InsertRect = 0 ;
    SendEllipse = 0;
    myState = noSendItem;
    prevMode = move;
    prevType = noDev;
    myOpen = false;
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
    switch ( nowMode ) { // Исходя из режима в котором находится сцена мы выполняем действия
            case noFile: // Сцена закрыта
                return;
            case cable: // Если режим кабель
                // Заново прорисовываем линию от начала кабеля до Текущей точки
                if (line != 0) line->setLine(QLineF(line->line().p1(), event->scenePos()));
                break;
            case move: // Если перемещение
                // Если есть перемещаемые устройства используем метод предка для их перемещения
                if ( coordMap.count() ) QGraphicsScene::mouseMoveEvent(event);
                else  if (SelectRect ) // Если есть выделение обновляем его.
                        SelectRect->setRect(QRectF( event->scenePos() , p2Rect ).normalized());
                break;
            case insert: // Если режим вставки устройства
                if ( InsertRect ) InsertRect->moving( event->scenePos() );
                break;
            case send: // Если режим отправки пакетов.
                if ( SendEllipse ) SendEllipse->moving( event->scenePos() );
                break;
    }
}
//----------------------------------------------------------------------
/*!
  Событие нажатия мыши.
  @param event - переменная события.
*/
void myCanvas::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if ( event->button() != Qt::LeftButton ) {
        QGraphicsScene::mousePressEvent(event);
        return;
    }
    switch (nowMode) {
        case noFile:
            return; // Если файл не открыт то не будем обрабатывать вообще
        case move: // если режим перемещения
            QGraphicsScene::mousePressEvent(event); // Сначала событие предка
            // Если есть выделенные элементы и мы щелкаем на одном из них
            if ( selectedItems().count() && items( event->scenePos()).count() ) {
                // То нужно сохранить все их координаты на случай если начнется перемещение.
                foreach ( QGraphicsItem* i , selectedItems() ) {
                    if ( i->type() != cableDev::Type )
                        coordMap.insert( i , i->scenePos());
                }
            } // Иначе создаем прямоугольник выделения.
            else {
                if ( items( event->scenePos() ).count() ) break;
                SelectRect = new selectRect;
                p2Rect = QPointF( event->scenePos() );
                addItem(SelectRect);
            }
            break;
        case text:
             createTextItem(event->scenePos());
             setMode( move , noDev );
             uncheck();
             break;
        case insert: // Если режим вставки
            if ( InsertRect->pos().y() < 0 ) break;
            if ( InsertRect->isReadyInsert() ) {
                addDeviceOnScene(event->scenePos(), nowType); // Добавляем устройство на сцену
                prevMode = insert;
                prevType = nowType;
            }
            break;
        case cable: // А если кабель
            if ( items(event->scenePos()).isEmpty()) break; // не будем водить кабеля из пустых мест
            line =  new QGraphicsLineItem(QLineF(event->scenePos(),
                                         event->scenePos()));
            //Создадим временную линию, выставим её свойства и добавим на сцену
            line->setPen(QPen(Qt::black,2));
            addItem(line);
            break;
        case send:
            if ( SendEllipse->hasUnderDevice() ) {
                QGraphicsItem *tempItem = SendEllipse->underDevice() ;
                device *t = qgraphicsitem_cast<device*>(tempItem);
                if ( !t ) break;
                if ( !t->isCanSend() ) {
                    QMessageBox::warning(0,tr("Error"),tr("The device can't transmit data!"),
                                     QMessageBox::Ok , QMessageBox::Ok);
                    break;
                }
                if ( myState == noSendItem ) {
                    sendDialog *temp = new sendDialog(sendDialog::sender,t);
                    if (temp->exec() ) {
                        messageSize = temp->messageSize();
                        broadcast = temp->broadcast();
                        protocol = temp->protocol();
                        senderDevice = t;
                        if ( broadcast ) {
                            emit uncheck();
                            setMode( myCanvas::move , myCanvas::noDev);
                        } else {
                            myState = oneSendItem;
                            SendEllipse->chooseOneDevice();
                        }
                    }
                    delete temp;
                } else {
                    sendDialog *temp = new sendDialog(sendDialog::receiver,t);
                    if (temp->exec() ) {
                        receiverIp = temp->dest();
                        senderDevice->sendMessage(receiverIp,messageSize,protocol);
                        emit uncheck();
                        setMode( myCanvas::move , myCanvas::noDev);
                    }
                    delete temp;
                }
            }
            break;
    }
}
//-----------------------------------------------------------------------
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
    return cable;
}
//-------------------------------------------------------------------------
void myCanvas::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if ( event->button() != Qt::LeftButton ) {
        QGraphicsScene::mouseReleaseEvent(event);
        return;
    }
    bool canCreate = true; // показывает можно ли
    switch (nowMode) {
            case noFile:
                return;
            case cable:
                if (line) { // Если кабель тянеться и режим тоже кабель
                    QString start,end;
                    device *startItem = deviceInPoint(line->line().p1()) ;
                    device *endItem = deviceInPoint(line->line().p2()) ;
                    removeItem(line);
                    delete line;
                    line = 0; // Линию временную делаем указателем на нуль
                    if ( !startItem || !endItem ) break;
                    if ( device::isConnectDevices(startItem, endItem) ) break;
                    if ( startItem == endItem ) break;
                    if ( canCreate ) {
                        connectDialog *conDialog = new connectDialog(startItem,endItem);
                        conDialog->move(400,400);
                        canCreate = conDialog->exec() ;
                        if (canCreate) {
                            start = conDialog->getStart();
                            end = conDialog->getEnd();
                        }
                        delete conDialog;
                    }
                    else canCreate = false;
                    // Вообщем так ... если уствойства есть под обоими концами
                    // и эти устройства различны то мы создаем этот кабель! Проверено все ок =)
                   if ( canCreate ) createConnection( startItem , endItem , start , end );
                   prevMode = cable;
                   prevType = noDev;
                }
                break;
            case move:
                if ( coordMap.count() ) {
                    bool needReturn = false;
                    QMapIterator<QGraphicsItem*,QPointF> i(coordMap);
                    i.toFront();
                    while (i.hasNext()) {
                         i.next();
                         QList<QGraphicsItem*> underItems = i.key()->collidingItems();
                         foreach ( QGraphicsItem *j , underItems )
                             if ( j->type() == cableDev::Type ) underItems.removeOne(j);
                         if ( i.key()->pos().x() < 0  || i.key()->pos().y() < 0 ||
                               i.key()->pos().x() > myCanvas::width || i.key()->pos().y() >  myCanvas::height
                                 || underItems.count() ) { needReturn = true; break; }
                    }
                    if ( needReturn ) {
                        i.toFront();
                         while (i.hasNext()) {
                            i.next();
                            i.key()->setPos(i.value());
                        }
                    }
                    i.toFront();
                    while (i.hasNext()) {
                        i.next();
                        if (i.key()->type() != textItem::Type ) i.key()->setPos( calibrate(i.key()->scenePos()) );
                        if ( isDevice(i.key()) ) {
                            device *d = qgraphicsitem_cast<device*>(i.key());
                            d->updateCables();
                        }
                    }
                    coordMap.clear();
                }
                else {
                    if ( SelectRect != 0 ) {
                        QPainterPath path;
                        path.addRect(SelectRect->rect());
                        setSelectionArea(path);
                        removeItem(SelectRect);
                        delete SelectRect;
                        p2Rect = QPoint();
                        SelectRect = 0 ;
                    }
                }
                QGraphicsScene::mouseReleaseEvent(event); // передаем событие предку
                break; 
            case insert:
                break;
            case text:
                QGraphicsScene::mouseReleaseEvent(event); // передаем событие предку
                break;
    }
}

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
    lastId = 0;
    setBackgroundBrush(QBrush(QPixmap(":im/images/back.png")));
    setSceneRect(0,0,myCanvas::width,myCanvas::height);
    setMode(myCanvas::move,myCanvas::noDev);
    myOpen = true;
    play();
    emit fileOpened();
}
//-------------------------------------------------
/*!
  Закрывает файл, очищает сцену, делает фон серым, удаляет все соединения.
*/
void myCanvas::closeFile()
{
    setMode(myCanvas::noFile,myCanvas::noDev);
    clear();
    myDevices.clear();
    setBackgroundBrush(QBrush(Qt::lightGray));
    setSceneRect(0,0,1,1);
    connections.clear();
    myTextItems.clear();
    if ( myTimer ) stop();
    myOpen = false;
    emit fileClosed();
}
//---------------------------------------------------
void myCanvas::deleteConnection(cableDev *cable)
{
    cable->deleteConnect();
    connections.removeOne(cable);
}

void myCanvas::setMode(int modScene,int curDev)
{
    nowMode = modScene;
    nowType = curDev;
    myState = noSendItem;
    setSelectionArea( QPainterPath() );
    if ( InsertRect ) {
        removeItem(InsertRect);
        delete InsertRect;
        InsertRect = 0 ;
    }
    if ( SendEllipse ) {
        removeItem( SendEllipse);
        delete SendEllipse;
        SendEllipse = 0;
    }
    switch ( nowMode ) {
        case insert:
            InsertRect = new insertRect;
            addItem(InsertRect);
            break;
        case send:
            SendEllipse = new sendEllipse;
            addItem(SendEllipse);
            break;
        default:
            break;
    }
}

void myCanvas::hideInsertRect()
{
    if ( InsertRect ) InsertRect->hideItem();
    if ( SendEllipse ) SendEllipse->hideItem();
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
        closeFile();
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
    killTimer(myTimer);
    myTimer = startTimer(n);
    appSetting::setAnimateSpeed(n);
}

void myCanvas::play()
{
    myTimer = startTimer( appSetting::animateSpeed() );
}





