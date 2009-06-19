#include "mycanvas.h"
#include "connectdialog.h"
#include "deviceport.h"
#include "frame.h"
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
#include <QTime>
#include <QGraphicsView>

myCanvas::myCanvas(QMenu *context, QObject *parent) : QGraphicsScene(parent)
{
    nowMode = noFile; // Сначала файла нет
    itemMenu = context; // меню из аргумента
    line = 0;
    selectRect = 0;
    p2Rect = QPoint();
    coordMap.clear();
    insertRect = 0 ;
    sendEllipse = 0;
    myState = noSendItem;
    prevMode = move;
    prevType = noDev;
    myOpen = false;
    myTimer = startTimer(100);
}

myCanvas::~myCanvas()
{

}

void myCanvas::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    switch ( nowMode ) {
            case noFile:
                return;
            case cable: // Если режим у нас кабель
                if (line != 0) line->setLine(QLineF(line->line().p1(), event->scenePos()));
                // Заново прорисовываем линию от начала кабеля до Текущей точки
                break;
            case move:
                if ( coordMap.count() )
                        QGraphicsScene::mouseMoveEvent(event);
                else  if (selectRect != 0 )
                        selectRect->setRect(QRectF( event->scenePos() , p2Rect ).normalized());
                break;
            case insert:
                if ( insertRect ) {
                    if ( insertRect->collidingItems().isEmpty() ) {
                        insertRect->setPen(QPen(Qt::blue));
                        insertRect->setBrush( QBrush( QColor( 0 , 0 , 128 , 64)));
                    }
                    else
                    {
                        insertRect->setPen(QPen(Qt::red));
                        insertRect->setBrush( QBrush( QColor( 128 , 0 , 0 , 64)));
                    }
                    insertRect->setPos( event->scenePos() );
                }
                break;
            case send:
                if ( sendEllipse ) {
                    if ( myState == noSendItem ) {
                        sendEllipse->setPen(QPen(Qt::lightGray));
                        sendEllipse->setBrush( QBrush(QColor(255,128,0,128)));
                    } else {
                        sendEllipse->setPen(QPen(Qt::green));
                        sendEllipse->setBrush( QBrush(QColor(0,128,0,128)));
                    }
                    sendEllipse->setPos( event->scenePos());
                }
                break;
    }
}

void myCanvas::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if ( event->button() != Qt::LeftButton ) {
        QGraphicsScene::mousePressEvent(event);
        return;
    }
    bool isCorrect = true;
    QList<QGraphicsItem*> col;
    switch (nowMode) {
        case noFile:
            return; // Если файл не открыт то не будем обрабатывать вообще
        case move:
            QGraphicsScene::mousePressEvent(event);
            if ( selectedItems().count() && !items( event->scenePos()).isEmpty()) {
                QList<QGraphicsItem*> devices = selectedItems();
                foreach ( QGraphicsItem* i , devices) {
                    if ( i->type() != cableDev::Type )
                        coordMap.insert( qgraphicsitem_cast<device*>(i) , i->scenePos());
                }
            }
            else {
                if ( !items( event->scenePos() ).isEmpty() ) break;
                selectRect = new QGraphicsRectItem;
                selectRect->setPos( 0,0 );
                p2Rect = QPointF( event->scenePos() );
                selectRect->setPen(QPen(Qt::blue));
                selectRect->setBrush( QBrush( QColor( 0 , 0 ,128 , 64)));
                selectRect->setZValue(1000);
                addItem(selectRect);
            }
            break;
        case insert: // Если режим вставки
            if ( insertRect->pos().y() < 0 ) break;
            isCorrect = insertRect->collidingItems().isEmpty();
            if (isCorrect) {
                addDeviceOnScene(event->scenePos(), nowType); // Добавляем устройство на сцену
                if ( insertRect ) {
                    removeItem(insertRect);
                    delete insertRect;
                    insertRect = 0 ;
                }
                prevMode = insert;
                prevType = nowType;
                emit uncheck();
                setMode( move , noDev );
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
                if (tempItem->type() != computer::Type) break;
                device* tempDev = qgraphicsitem_cast<device*>(tempItem);
                if ( !tempDev->isConnect() ) {
                QMessageBox::warning(NULL,trUtf8("Ошибка"),trUtf8("Устройство не имеет соединений"),
                                     QMessageBox::Ok , QMessageBox::Ok);
                    break;
                }
                if ( myState == noSendItem ) {
                    sendDialog temp(sendDialog::sender,tempDev);
                    temp.move(400,400);
                    if (temp.exec() ) {
                        messageSize = temp.messageSize();
                        broadcast = temp.broadcast();
                        protocol = temp.protocol();
                        senderDevice = temp.senderDevice();
                        if ( broadcast ) {
                            emit uncheck();
                            setMode( myCanvas::move , myCanvas::noDev);
                        } else myState = oneSendItem;
                    }
                } else {
                    sendDialog temp(sendDialog::receiver,tempDev);
                    temp.move(400,400);
                    if (temp.exec() ) {
                        receiverIp = temp.ip();
                        smartDevice *s = senderDevice->toT<smartDevice>() ;
                        s->sendMessage(receiverIp,messageSize,protocol);
                        emit uncheck();
                        setMode( myCanvas::move , myCanvas::noDev);
                    }
                }
            }
            break;
    }
}

cableDev* myCanvas::createConnection(device *s , device *e , QString sp,QString ep)
{
    if ( !s || !e ) return NULL;
    devicePort *ts = s->socket(sp);
    devicePort *te = e->socket(ep);
    cableDev *cable = new cableDev(s, e, ts, te); // Создаем между ними кабель
    s->update(s->boundingRect());
    e->update(e->boundingRect());
    cable->setZValue(-1000.0); // Кидаем его на самый-самый задний план
    addItem(cable); // И добавляем его на сцену =)
    ts->setConnect(true,cable);
    te->setConnect(true,cable);
    connections << cable;
    s->addConnection(cable);
    e->addConnection(cable);
    cable->updatePosition(); // Обновляем его положение
    return cable;
}

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
                    if ( startItems.count() > 0 && endItems.count() > 0) {
                        foreach ( cableDev* cabledev, connections )
                            if ( (cabledev->start() == qgraphicsitem_cast<device*>(startItems.first()) &&
                                  cabledev->end() == qgraphicsitem_cast<device*>(endItems.first()) ) ||
                                 ( cabledev->start() == qgraphicsitem_cast<device*>(endItems.first()) &&
                                   cabledev->end() == qgraphicsitem_cast<device*>(startItems.first()) )
                                ) { canCreate = false; }
                    } else canCreate = false;
                    if (canCreate && startItems.first() != endItems.first() &&
                        startItems.first()->type() != cableDev::Type ) {
                        startItem = qgraphicsitem_cast<device *>(startItems.first());
                        endItem = qgraphicsitem_cast<device *>(endItems.first());
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
                nowMode = move;
                emit uncheck();
                break;
            case move:
                if ( coordMap.count() ) {
                    bool needReturn = false;
                    QMapIterator<device*,QPointF> i(coordMap);
                    i.toFront();
                    while (i.hasNext()) {
                         i.next();
                         QList<QGraphicsItem*> underItems = i.key()->collidingItems();
                         foreach ( QGraphicsItem* item , underItems)
                             if ( item->type() == cableDev::Type ) underItems.removeOne(item);
                         if ( i.key()->pos().x() < 0  || i.key()->pos().y() < 0 ||
                               i.key()->pos().x() > myCanvas::width || i.key()->pos().y() >  myCanvas::height
                                 || underItems.count() > 0) { needReturn = true; break; }
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
                        i.key()->setPos( calibrate(i.key()->scenePos()) );
                        foreach ( cableDev* itemka, i.key()->cables() )
                                itemka->updatePosition();
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
    }
}

device* myCanvas::addDeviceOnScene(QPointF coor, int myType)
{
    device *item = 0; // Указатель на абстрактное устройств
    int x = qRound(coor.x())/50;
    int y = qRound(coor.y())/50;
    switch (myType) {
        case compDev:
            item = createDev<computer>(x,y,myComputerSockets);
            item->setToolTip(trUtf8("Компьютер"));
            break;
        case hubDev: // Для остальных устройств все тоже самое =)
            item = createDev<hubDevice>(x,y,myHubSockets);
            item->setToolTip( trUtf8( "Концентратор" ) );
            item->toT<hubDevice>()->setManual(myHubManual);
            break;
        case switchDev:
            item = createDev<switchDevice>(x,y,mySwitchSockets); // Создаем конкретное устройство коммутатор
            item->setToolTip(trUtf8("Коммутатор"));
            item->toT<switchDevice>()->setManual(mySwitchManual);
            break;
        case routerDev:
            item = createDev<routerDevice>(x,y,myRouterSockets);
            item->setToolTip(trUtf8("Маршрутизатор"));
            break;
        case busDev:
            item = createDev<shareBus>(x,y,0); // Общая шина
            item->setToolTip(trUtf8("Общая шина"));
            break;
        case noDev: return NULL;
            break;
    }
    return item;
}

void myCanvas::removeDevice()
{
    QList<QGraphicsItem*> lostItems = selectedItems();
    foreach (QGraphicsItem *item, lostItems) {
        if ( item->type() != cableDev::Type ) {
            device* temp = qgraphicsitem_cast<device*>(item);
            QList<cableDev*> lostCables = temp->cables();
            foreach ( cableDev* itemka , lostCables) {
                removeItem(itemka);
                deleteConnection(itemka);
            }
        }
        else deleteConnection(qgraphicsitem_cast<cableDev*>(item));
        removeItem(item);
    }
}

void myCanvas::newFile()
{
    setBackgroundBrush(QBrush(QPixmap(":im/images/back.png")));
    setSceneRect(0,0,myCanvas::width,myCanvas::height);
    setMode(myCanvas::move,myCanvas::noDev);
    myOpen = true;
}

void myCanvas::closeFile()
{
    setMode(myCanvas::noFile,myCanvas::noDev);
    clear();
    setBackgroundBrush(QBrush(Qt::lightGray));
    setSceneRect(0,0,1,1);
    connections.clear();
    myOpen = false;
}

void myCanvas::deleteConnection(cableDev *cable)
{
    cable->startPort()->setConnect(false,cable);
    cable->endPort()->setConnect(false,cable);
    cable->start()->deleteConnection(cable);
    cable->start()->update(cable->start()->boundingRect());
    cable->end()->deleteConnection(cable);
    cable->end()->update(cable->end()->boundingRect());
    cable->startPort()->setConnect(false,NULL);
    cable->endPort()->setConnect(false,NULL);
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
            insertRect->setPen(QPen(Qt::red));
            insertRect->setBrush( QBrush( QColor( 128 , 0 , 0 , 64)));
            insertRect->setZValue(1000);
            addItem(insertRect);
            if ( nowType == compDev || nowType == switchDev || nowType == hubDev || nowType == routerDev) {
                insertRect->setRect( -20 , -20 , 40 ,40);
            } else if ( nowType == busDev ) {
                insertRect->setRect( -shareBus::defaultWidth/2 -10 , -20 , shareBus::defaultWidth+20 , 40);
            }
            break;
        case send:
            sendEllipse = new QGraphicsEllipseItem;
            sendEllipse->setPos( 0 , -100);
            sendEllipse->setPen(QPen(Qt::blue));
            sendEllipse->setBrush(QBrush( QColor( 0 , 0 ,128 ,64)));
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

void myCanvas::openScene(QString fileName)
{
    newFile();
    bool endDev = true;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Невозможно открыть файл для чтения";
        return;
    }
    QDataStream s(&file);
    s.setVersion(QDataStream::Qt_4_2);
    int tp;
    QPointF p;
    QString str;
    device *item;
    int n,i;
    while ( !file.atEnd() && endDev ) {
        s >> tp;
        if ( tp != noDev ) {
            item = addDeviceOnScene(QPointF(),tp);
            s >> *item;
        }
        else endDev = false;
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
    if ( s.status() != QDataStream::Ok ) qDebug() << "PPC";
    file.close();
    qDebug() << QString("Scene was been open from %1").arg(fileName) ;
}

void myCanvas::saveScene(QString fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Невозможно открыть файл для записи";
        return;
    }
    QDataStream s(&file);
    s.setVersion(QDataStream::Qt_4_2);
    device *item;
    QList<QGraphicsItem*> list = items();
    foreach(QGraphicsItem *t, list) {
        if (t->type() == cableDev::Type) continue;
        item = qgraphicsitem_cast<device*>(t);
        s << *item;
    }
    s << noDev;
    s << connections.count();
    foreach (cableDev *tempCable, connections) {
        s << tempCable->line().p1() << tempCable->line().p2();
        s << tempCable->startPort()->name() << tempCable->endPort()->name() ;
    }
    if ( s.status() != QDataStream::Ok ) qDebug() << "PPC";
    file.close();
    qDebug() << QString("Scene was been saved in %1").arg(fileName) ;
}

void myCanvas::keyPressEvent(QKeyEvent *event)
{
    if ( sceneRect().size().width() < 1000 ) return;
    if ( event->nativeVirtualKey() == 32 ) {
        setMode( prevMode , prevType );
    }
    event->accept();
}

void myCanvas::timerEvent(QTimerEvent *e)
{
    Q_UNUSED(e);
    static int n = 10;
    motionFrame();
    if ( --n == 0 ) {
        QList<QGraphicsItem*> l = items();
        foreach ( QGraphicsItem *i, l ) {
            if ( i->type() == routerDevice::Type || i->type() == computer::Type ){
                smartDevice *t = qgraphicsitem_cast<smartDevice*>(i);
                t->updateArp(myTtlArp);
                t->sendRip(myRip);
            } else if ( i->type() == switchDevice::Type ) {
                switchDevice *t = qgraphicsitem_cast<switchDevice*>(i);
                t->updateMac(myTtlMac);
            }
        }
        n = 10;
    }
}

void myCanvas::motionFrame()
{
    foreach ( cableDev *t , connections) {
        if ( t->isBusy() )
            t->motion();
    }
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
    if ( selectedItems().count() == 1 && selectedItems().first()->type() != cableDev::Type )
        return qgraphicsitem_cast<device*>(selectedItems().first());
    return NULL;
}

device* myCanvas::deviceInPoint(QPointF p)
{
    if ( items(p).isEmpty() || items(p).first()->type() == cableDev::Type ) return NULL;
    return qgraphicsitem_cast<device*>(items(p).first());
}


void myCanvas::setShowGrid(bool b)
{
    if (b) setBackgroundBrush(QBrush(QPixmap(":im/images/back.png")));
    else setBackgroundBrush(QBrush(Qt::white));
}

QPointF myCanvas::calibrate(QPointF c)
{
    c.setX( (qRound(c.x()) / 50)*50+25 );
    c.setY( (qRound(c.y()) / 50)*50+25 );
    return c;
}

void myCanvas::setTest(bool c)
{
    Q_UNUSED(c)
    return;
}
