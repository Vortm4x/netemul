#include "mycanvas.h"
#include "connectdialog.h"
#include "device.h"
#include "cabledev.h"
#include "senddialog.h"
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
    selectRect = 0; // Выделения нет
    p2Rect = QPoint();
    coordMap.clear();
    myDevices.clear();
    insertRect = 0 ;
    sendEllipse = 0;
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
                else  if (selectRect != 0 ) // Если есть выделение обновляем его.
                        selectRect->setRect(QRectF( event->scenePos() , p2Rect ).normalized());
                break;
            case insert: // Если режим вставки устройства
                if ( insertRect ) {
                    if ( insertRect->collidingItems().isEmpty() )
                        setShapeView( insertRect , QPen(Qt::blue) , QBrush( QColor( 0 , 0 , 128 , 64) ) );
                    else
                        setShapeView( insertRect , QPen(Qt::red) , QBrush( QColor( 128 , 0 , 0 , 64)) );
                    insertRect->setPos( event->scenePos() );
                }
                break;
            case send: // Если режим отправки пакетов.
                if ( sendEllipse ) {
                    if ( myState == noSendItem )
                        setShapeView(sendEllipse , QPen(Qt::lightGray), QBrush(QColor(255,128,0,128)));
                    else
                        setShapeView(sendEllipse , QPen(Qt::green) , QBrush(QColor(0,128,0,128)));
                    sendEllipse->setPos( event->scenePos());
                }
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
    bool isCorrect = true;
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
                selectRect = new QGraphicsRectItem;
                selectRect->setPos( 0,0 );
                p2Rect = QPointF( event->scenePos() );
                setShapeView( selectRect , QPen(Qt::blue) , QBrush( QColor( 0 , 0 ,128 , 64)) );
                selectRect->setZValue(1000);
                addItem(selectRect);
            }
            break;
        case text:
             textRect = createTextItem();
             textRect->setPos(event->scenePos());
             setMode( move , noDev );
             uncheck();
             break;
        case insert: // Если режим вставки
            if ( insertRect->pos().y() < 0 ) break;
            isCorrect = insertRect->collidingItems().isEmpty();
            if (isCorrect) {
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
            if ( sendEllipse->collidingItems().count() ) {
                QGraphicsItem *tempItem = sendEllipse->collidingItems().first();
                device *t = qgraphicsitem_cast<device*>(tempItem);
                if ( !t ) break;
                if ( !t->isCanSend() ) break;
                if ( !t->isConnect() ) {
                QMessageBox::warning(NULL,trUtf8("Ошибка"),trUtf8("Устройство не имеет соединений"),
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
                        } else myState = oneSendItem;
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
    cable->setZValue(-1000.0); // Кидаем его на самый-самый задний план
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
                if (line != 0) { // Если кабель тянеться и режим тоже кабель
                    device *startItem = 0;
                    device *endItem = 0;
                    QString start,end;
                    QList<QGraphicsItem *> startItems = items(line->line().p1());
                    if (startItems.count() && startItems.first() == line)
                        startItems.removeFirst(); // если первое устройство под одним концом эта же линия
                            // то удаляем её =) Все тоже самое для второго конца
                    QList<QGraphicsItem *> endItems = items(line->line().p2());
                    if (endItems.count() && endItems.first() == line)
                        endItems.removeFirst();
                        // Удаляем временную линию и освобождаем её память
                    removeItem(line);
                    if ( startItems.count()  && endItems.count() ) {
                        foreach ( cableDev* cabledev, connections )
                            if ( (cabledev->start() == qgraphicsitem_cast<device*>(startItems.first()) &&
                                  cabledev->end() == qgraphicsitem_cast<device*>(endItems.first()) ) ||
                                 ( cabledev->start() == qgraphicsitem_cast<device*>(endItems.first()) &&
                                   cabledev->end() == qgraphicsitem_cast<device*>(startItems.first()) )
                                ) { canCreate = false; }
                    } else canCreate = false;
                    if (canCreate && startItems.first() != endItems.first() && isDevice(startItems.first() ) ) {
                        startItem = qgraphicsitem_cast<device*>(startItems.first());
                        endItem = qgraphicsitem_cast<device*>(endItems.first());
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
                }
                delete line;
                line = NULL; // Линию временную делаем указателем на нуль
                prevMode = cable;
                prevType = noDev;
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
                        //qDebug() << "Nado iz" << i.key()->scenePos() << " v " << i.value() << endl;
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
                            foreach ( cableDev* itemka, d->cables() )
                                    itemka->updatePosition();
                        }
                    }
                    coordMap.clear();
                }
                else {
                    if ( selectRect != 0 ) {
                        QPainterPath path;
                        path.addRect(selectRect->rect());
                        setSelectionArea(path);
                        removeItem(selectRect);
                        delete selectRect;
                        p2Rect = QPoint();
                        selectRect = 0 ;
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
    if ( insertRect ) {
        removeItem(insertRect);
        delete insertRect;
        insertRect = 0 ;
    }
    if ( sendEllipse ) {
        removeItem( sendEllipse);
        delete sendEllipse;
        sendEllipse = 0;
    }
    switch ( nowMode ) {
        case insert:
            insertRect = new QGraphicsRectItem;
            insertRect->setPos( 0, -50 );
            setShapeView(insertRect,QPen(Qt::red),QBrush( QColor( 128 , 0 , 0 , 64)));
            insertRect->setZValue(1000);
            addItem(insertRect);
            insertRect->setRect( -20 , -20 , 40 ,40);
            break;
        case send:
            sendEllipse = new QGraphicsEllipseItem;
            sendEllipse->setPos( 0 , -100);
            setShapeView(sendEllipse,QPen(Qt::blue),QBrush( QColor( 0 , 0 ,128 ,64)));
            sendEllipse->setZValue(1000);
            addItem(sendEllipse);
            sendEllipse->setRect(-10,-10,20,20);
            break;
        default:
            break;
    }
}

void myCanvas::hideInsertRect()
{
    if ( insertRect ) insertRect->setPos( 100 , -100);
    if ( sendEllipse ) sendEllipse->setPos( 100 , -100);
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
        QMessageBox::critical(NULL,trUtf8("Ошибка"),trUtf8("Устаревшая версия файла сохранения. Файл не может быть открыт."),
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
        textItem *t = createTextItem();
        s >> p; t->setPos(p);
        s >> str; t->setPlainText(str);
        t->adjustSize();
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
    s << noDev;
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
void myCanvas::timerEvent(QTimerEvent *e)
{
    Q_UNUSED(e);
    static int n = 10;
    motionFrame();
    n--;
    foreach ( device *i, myDevices ) {
        i->deciSecondTimerEvent();
        if ( !n ) i->secondTimerEvent();
    }
    if ( !n ) n = 10;
}
//--------------------------------------------------------------
void myCanvas::motionFrame()
{
    foreach ( cableDev *t , connections)
            if ( t->isBusy() ) t->motion();
}

void myCanvas::whileMotion()
{
    while ( !isEnd() ) motionFrame();
}

bool myCanvas::isEnd() const
{
    foreach ( cableDev *t , connections ) {
        if ( t->isBusy() ) return false;
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
    if ( !itemAt(p) || !isDevice(itemAt(p)) ) return NULL;
    return qgraphicsitem_cast<device*>(items(p).first());
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
/*!
  Задает указанному объекту внешний вид.
  @param i - указатель на объект.
  @param p - перо для объекта.
  @param b - кисть для объекта.
*/
void myCanvas::setShapeView(QAbstractGraphicsShapeItem *i , QPen p , QBrush b)
{
    i->setPen(p);
    i->setBrush(b);
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
textItem* myCanvas::createTextItem()
{
    textItem *t = new textItem;
    t->setTextInteractionFlags(Qt::TextEditorInteraction);
    t->setZValue(1000.0);
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
    if ( t->type() != cableDev::Type && t->type() != textItem::Type ) return true;
    return false;
}
//------------------------------------------------------------------------

deviceImpl* myCanvas::createComputer()
{
    device *t = addDeviceOnScene(QPointF(25,25) , compDev);
    return t->contentDevice();
}




