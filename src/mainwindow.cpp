#include "mainwindow.h"
#include "mycanvas.h"
#include "settingdialog.h"
#include "testdialog.h"
#include "interfacedialog.h"
#include "computer.h"
#include "routerdevice.h"
#include "switchdevice.h"
#include <QGraphicsView>
#include <QApplication>
#include <QDockWidget>
#include <QMenu>
#include <QAction>
#include <QActionGroup>
#include <QStatusBar>
#include <QMessageBox>
#include <QMenuBar>
#include <QEvent>
#include <QCloseEvent>
#include <QSettings>
#include <QFileDialog>
#include <QtDebug>
#include <QToolBar>
#include <QComboBox>

#ifndef QT_NO_OPENGL
#include <QtOpenGL/QtOpenGL>
#else
#endif


// Конструктор главной формы
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createAction(); // Создаем события
    createTools(); //
    createMenu(); // Создаем меню
    createScene(); // Создаем сцену
    setEnabledFileItems( myFile.length() );
    view = new QGraphicsView(canva,this);
    view->setFocus(); // Даем ему фокус
    view->setRenderHint(QPainter::Antialiasing); // Включаем сглаживание
    view->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
#ifndef QT_NO_OPENGL
#if USER != frost
    view->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
#endif
#else
#endif
    view->installEventFilter(this);
    statusBar()->showMessage(tr("")); // Активизируем статус бар
    readSetting();
    setCentralWidget(view);
}

MainWindow::~MainWindow()
{

}

//Событие закрытия
void MainWindow::closeEvent(QCloseEvent *event)
{
    // При попытке закрыть главное окно выводим запрос на подтверждение
    int res = QMessageBox::question(this,trUtf8("Выход"),trUtf8("Вы дейсвительно хотите выйти"),
                                    QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if (res == QMessageBox::Yes) {
        writeSetting();
        event->accept();
    }
    else event->ignore();
}

// Создать один Action
QAction* MainWindow::createOneAction(QString text,QString tip,QIcon icon/*= QIcon()*/,bool inGroup /*=false*/)
{
    QAction *temp = new QAction(text,this);
    temp->setStatusTip(tip);
    temp->setIcon(icon);
    if (inGroup) {
        groupAct->addAction(temp);
        temp->setCheckable(true);
    }
    return temp;
}

//Создаем элементы меню
void MainWindow::createAction()
{
    groupAct = new QActionGroup(this);
    groupAct->setExclusive(true);
    connect( groupAct , SIGNAL(triggered(QAction*)) , SLOT(groupClicked(QAction*)));

    newAct = createOneAction(trUtf8("Новый"),trUtf8("Создать новую сеть"));
    newAct->setShortcut(QKeySequence::New); // Устанавливаем горячие клавиши
    connect( newAct, SIGNAL(triggered()) , SLOT(newFile()));

    openAct = createOneAction(trUtf8("Открыть"),trUtf8("Открыть существующий файл"));
    openAct->setShortcut(QKeySequence::Open);
    connect( openAct , SIGNAL(triggered()) , SLOT(openFile()));

    saveAct = createOneAction(trUtf8("Сохранить"),trUtf8("Сохранить созданую сеть"));
    saveAct->setShortcut(QKeySequence::Save);
    connect( saveAct , SIGNAL(triggered()) , SLOT(saveFile()) );

    saveAsAct = createOneAction(trUtf8("Сохранить как..."),trUtf8("Сохранить сеть как..."));
#if QT_VERSION >= 0x040500
    saveAsAct->setShortcut(QKeySequence::SaveAs);
#endif
    connect( saveAsAct , SIGNAL(triggered()) , SLOT(saveAsFile()) );

    closeAct = createOneAction(trUtf8("Закрыть"),trUtf8("Закрыть текущий файл"));
    closeAct->setShortcut(QKeySequence::Close);
    connect( closeAct , SIGNAL(triggered())  , SLOT(closeFile()));

    exitAct = createOneAction(trUtf8("Выход"),trUtf8("Выйти из приложения"));
    exitAct->setShortcut(tr("Ctrl+X"));
    connect( exitAct , SIGNAL(triggered()) , SLOT(close()) );

    showGridAct = createOneAction(trUtf8("Показать сетку"),trUtf8("Показать сетку"));
    showGridAct->setCheckable(true);
    showGridAct->setChecked(true);

    deleteAct = createOneAction(trUtf8("Удалить"),trUtf8("Удалить объект") , QIcon(":/im/images/minus2.png"));
    deleteAct->setShortcut(QKeySequence::Delete);

    settingAct = createOneAction( trUtf8("Настройки") , trUtf8("Настройки"));
    connect( settingAct , SIGNAL(triggered()) , SLOT(setting()));

    staticsAct = createOneAction( trUtf8("Статистика") , trUtf8("Посмотреть статистику сцены") );

    moveAct = createOneAction( trUtf8("Перемещение"),trUtf8("Перемещение объектов"),
                               QIcon(":/im/images/arrow.png"),true);
    moveAct->setData( myCanvas::move*10 + myCanvas::noDev );
    moveAct->setChecked(true);

    cableAct = createOneAction( trUtf8("Кабель"),trUtf8("Создать соединение"),
                            QIcon(":/im/images/cable.png"),true);
    cableAct->setData( myCanvas::cable*10 + myCanvas::noDev );

    shareBusAct = createOneAction( trUtf8("Общая шина"),trUtf8("Добавить общую шину"),
                                    QIcon(":/im/images/sharebus.png"),true);
    shareBusAct->setData( myCanvas::insert*10 + myCanvas::busDev);

    computerAct = createOneAction( trUtf8("Компьютер"),trUtf8("Добавить компьютер"),
                               QIcon(":/im/images/laptop.png"),true);
    computerAct->setData( myCanvas::insert*10 + myCanvas::compDev);

    hubAct = createOneAction( trUtf8("Концентратор"),trUtf8("Добавить концентратор"),
                          QIcon(":/im/images/modem.png"),true);
    hubAct->setData( myCanvas::insert*10 + myCanvas::hubDev );

    switchAct = createOneAction( trUtf8("Коммутатор"),trUtf8("Добавить коммутатор"),
                                QIcon(":/im/images/switch.png"),true);
    switchAct->setData( myCanvas::insert*10 + myCanvas::switchDev );

    routerAct = createOneAction( trUtf8("Маршрутизатор") , trUtf8("Добавить маршрутизатор") ,
                                 QIcon(":/im/images/router.png") , true);
    routerAct->setData( myCanvas::insert*10 + myCanvas::routerDev );

    sendAct = createOneAction( trUtf8("Отправка"),trUtf8("Отправить данные"),
                           QIcon(":/im/images/left_right.png"),true);
    sendAct->setData( myCanvas::send* 10 + myCanvas::noDev);

    testAct = createOneAction( trUtf8("Тестирование") , trUtf8("Протестировать программу") );
    connect( testAct , SIGNAL(triggered()) , SLOT(test()));

    addPortAct = createOneAction( trUtf8("Добавить адаптер") , trUtf8("Добавить адаптер") ,
                                  QIcon(":/im/images/edit_add.png") );
    connect( addPortAct , SIGNAL(triggered()) , SLOT(addInterface()) );

    editPortAct = createOneAction( trUtf8("Свойства") , trUtf8("Свойства") ,
                                  QIcon(":/im/images/refresh.png") );
    connect( editPortAct , SIGNAL(triggered()) , SLOT(editInterface()) );

    removePortAct = createOneAction( trUtf8("Удалить адаптер") , trUtf8("Удалить адаптер") ,
                                  QIcon(":/im/images/edit_remove.png") );
    connect( removePortAct , SIGNAL(triggered()) , SLOT(removeInterface()) );

    tableAct = createOneAction( trUtf8("Таблица маршрутизации") , trUtf8("Редактирование таблицы маршрутизации"));
    connect( tableAct , SIGNAL(triggered()) , SLOT(tableShow()) );

    aboutQtAct = createOneAction( trUtf8("About Qt") , trUtf8("Справочная информация о Qt") );
    connect( aboutQtAct , SIGNAL(triggered()) , qApp ,SLOT(aboutQt()) );

    aboutAct = createOneAction( trUtf8("О программе") ,trUtf8("О программе") );    

    helpAct = createOneAction( trUtf8("Помощь") , trUtf8("Полная справочная система программы") );
    helpAct->setShortcut(QKeySequence::HelpContents);
    connect(helpAct,SIGNAL(triggered()) , SLOT(showHelp()));

    adapterAct = createOneAction( trUtf8("Интерфейсы") , trUtf8("Редактировать интерфейсы") );
    connect( adapterAct , SIGNAL(triggered()) , SLOT(adapterShow()) );

    // Изначально сеть остановлена
    playAct = createOneAction( trUtf8("Запустить") , trUtf8("Запустить симуляцию сцены") ,
                               QIcon(":/im/images/play.png") );
    connect( playAct , SIGNAL(triggered()) ,SLOT(playBack()) );
}

//Создаем меню
void MainWindow::createMenu()
{
    // Создаем наши меню и добавляем их на меню бар,
    fileMenu = menuBar()->addMenu(trUtf8("Файл"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addAction(closeAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    viewMenu = menuBar()->addMenu(trUtf8("Вид"));
    viewMenu->addAction( deviceBar->toggleViewAction() );
    viewMenu->addAction( controlBar->toggleViewAction() );
    viewMenu->addAction(showGridAct);

    itemMenu = menuBar()->addMenu(trUtf8("Объект"));
    itemMenu->addAction(deleteAct);
    itemMenu->addAction(editPortAct);
    itemMenu->addAction(tableAct);
    itemMenu->addAction(adapterAct);
    itemMenu->setEnabled(false);

    settingMenu = menuBar()->addMenu( trUtf8("Сервис") );
    settingMenu->addAction(staticsAct);
    settingMenu->addAction(settingAct);

    testMenu = menuBar()->addMenu( trUtf8("Тест") );
    testMenu->addAction( testAct );

    helpMenu = menuBar()->addMenu( trUtf8("Справка") );
    helpMenu->addAction(helpAct);
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

// Создаем
void MainWindow::createTools()
{
    deviceBar = addToolBar(trUtf8("Устройства"));
    deviceBar->setIconSize(QSize(32,32));
    deviceBar->addAction( moveAct);
    deviceBar->addAction( cableAct);
    deviceBar->addAction( shareBusAct);
    deviceBar->addAction( computerAct);
    deviceBar->addAction( hubAct);
    deviceBar->addAction( switchAct);
    deviceBar->addAction( routerAct);
    deviceBar->addAction( sendAct);
    deviceBar->addSeparator();
    deviceBar->addAction( playAct);
    //deviceBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    deviceBar->setEnabled(false);
    cb_ports = new QComboBox(this);
    cb_ports->setFixedWidth(100);
    controlBar = addToolBar(trUtf8("Управление"));
    controlBar->setIconSize(QSize(32,32));
    controlBar->addWidget(cb_ports);
    controlBar->addAction(addPortAct);
    controlBar->addAction(removePortAct);
    controlBar->addSeparator();
    controlBar->addAction(editPortAct);
    controlBar->addSeparator();
    controlBar->setEnabled(false);
}

// Создаем сцену
void MainWindow::createScene()
{
    canva = new myCanvas(itemMenu,this); // Создаем сцену
    canva->setBackgroundBrush(QBrush(Qt::lightGray));
    canva->setSceneRect(0,0,10,10);
    connect( showGridAct , SIGNAL(toggled(bool)) , canva , SLOT(setShowGrid(bool)));
    connect( canva , SIGNAL(uncheck()) , SLOT(uncheck()));
    connect( deleteAct , SIGNAL(triggered()) , canva , SLOT(removeDevice()));
    connect( newAct , SIGNAL(triggered()) , canva , SLOT(newFile()));
    connect( closeAct , SIGNAL(triggered()) , canva , SLOT(closeFile()));
    connect( canva , SIGNAL(selectionChanged()) , SLOT(selectionChange()));
}


// Слот , создать новый файл
void MainWindow::newFile()
{
    setWindowTitle( myFile = trUtf8("new.net") );
    setEnabledFileItems(true);
    showGridAct->setChecked(true);
}

// Слот закрыть файл
void MainWindow::closeFile()
{
    setWindowTitle(myFile = trUtf8(""));
    setEnabledFileItems(false);
}

// Слот включает или отключает пункты меню в зависимости от того открыт файл или закрыт
void MainWindow::setEnabledFileItems(bool cur)
{
    deviceBar->setEnabled(cur);
    closeAct->setEnabled(cur);
    openAct->setEnabled(!cur);
    newAct->setEnabled(!cur);
    saveAct->setEnabled(cur);
    saveAsAct->setEnabled(cur);
    showGridAct->setEnabled(cur);
}

//Слот вызываемый при изменении выделения на сцене
void MainWindow::selectionChange()
{
    cb_ports->clear();
    if ( canva->oneSelectedDevice() ) {
        device *t = canva->oneSelectedDevice();
        QList<devicePort*> d = t->sockets();
        controlBar->setEnabled(true);
        tableAct->setVisible( !t->hasTable().isEmpty() );
        tableAct->setText( t->hasTable() );
        tableAct->setToolTip( t->hasTable() );
        adapterAct->setVisible( t->type() == computer::Type || t->type() == routerDevice::Type );
        foreach ( devicePort *i , d ) cb_ports->addItem( i->connectIcon() , i->name() );
        addPortAct->setEnabled(false);
        editPortAct->setEnabled(true);
        removePortAct->setEnabled(false);
        switch (t->type() ) {
            case computer::Type:
                addPortAct->setEnabled(true);
                editPortAct->setEnabled( !t->sockets().isEmpty() );
                removePortAct->setEnabled( !t->sockets().isEmpty() );
                break;
        }
    }
    else {
        controlBar->setEnabled(false);
    }
    itemMenu->setEnabled( canva->selectedItems().count() );
}

// Слот окна настроек
void MainWindow::setting()
{
    settingDialog *d = new settingDialog;
    connect( d , SIGNAL(sendApply()) , SLOT(applySetting()) );
    d->setHubSockets( canva->hubSockets() );
    d->setComputerSockets( canva->computerSockets() );
    d->setSwitchSockets( canva->switchSockets() );
    d->setSwitchManual(canva->switchManual());
    d->setHubManual(canva->hubManual());
    d->setRouterSockets(canva->routerSockets());
    d->setTtlArp(canva->ttlArp());
    d->setTtlMac(canva->ttlMac());
    d->setRip(canva->rip());
    d->applyDisable();
    if ( d->exec() ) {
        d->apply();
    }
    disconnect( d ,SIGNAL(sendApply()) , this ,  SLOT(applySetting()));
    delete d ;
}

void MainWindow::applySetting()
{
    settingDialog *t = qobject_cast<settingDialog*>(sender());
    canva->setComputerSockets( t->computerSockets() );
    canva->setHubSockets( t->hubSockets() );
    canva->setSwitchSockets( t->switchSockets() );
    canva->setHubManual(t->huManual() );
    canva->setSwitchManual(t->switchManual());
    canva->setRouterSockets(t->routerSockets());
    canva->setTtlArp(t->ttlArp());
    canva->setTtlMac(t->ttlMac());
    canva->setRip(t->rip());
}

//Слот сохранить
void MainWindow::saveFile()
{
    if ( myFile == trUtf8("new.net") ) {
       QString t = QFileDialog::getSaveFileName(this,trUtf8("Сохранить файл как ..."),
                                                QDir::currentPath(),
                                                trUtf8("Сети(*.net)"));
       if ( t.isEmpty() ) return;
       myFile = t;
    }
    setWindowTitle( myFile );
    canva->saveScene(myFile);
}

void MainWindow::openFile()
{
    QString t = QFileDialog::getOpenFileName(this,trUtf8("Открыть"),
                                             QDir::currentPath(),trUtf8("Сети(*.net)"));
    if ( t.isEmpty() ) return;
    myFile = t;
    setWindowTitle( myFile );
    setEnabledFileItems(true);
    showGridAct->setChecked(true);
    canva->openScene(myFile);
}

//Слот сохранить как =)
void MainWindow::saveAsFile()
{
    QString t = QFileDialog::getSaveFileName(this,trUtf8("Сохранить файл как ..."),
                                             QApplication::applicationDirPath(),trUtf8("Сети(*.net)"));
    if ( t.isEmpty() ) return ;
    myFile = t;
    setWindowTitle( myFile );
    canva->saveScene(myFile);
}

// Фильтр для виев айтем =)
bool MainWindow::eventFilter(QObject *obj,QEvent *event)
{
    if ( obj == view ) {
        if ( event->type() == QEvent::Leave )
            canva->hideInsertRect();
        return false;
    } else
    return QMainWindow::eventFilter(obj,event);
}
/*
  Запись настроек программы в файл.
*/
void MainWindow::writeSetting()
{
    QSettings setting("FROST","netemul");
    setting.beginGroup("window");
    setting.setValue("width", width() );
    setting.setValue("height", height() );
    setting.setValue("left" , pos().x() );
    setting.setValue("top" , pos().y() );
    setting.endGroup();
    setting.setValue("computer/socketCount" , canva->computerSockets() );
    setting.setValue("hub/socketCount" , canva->hubSockets() );
    setting.setValue("switch/socketCount" , canva->switchSockets() );
    setting.setValue("router/socketCount" , canva->routerSockets() );
    setting.setValue("hub/manual",canva->hubManual());
    setting.setValue("switch/manual", canva->switchManual());
    setting.setValue("ttl/Arp", canva->ttlArp());
    setting.setValue("ttl/Mac",canva->ttlMac());
    setting.setValue("ttl/Rip",canva->rip());
}
//---------------------------------------------------
/*
  Функция чтения настроек из файла.
*/
void MainWindow::readSetting()
{
    QSettings setting("FROST","netemul");
    setting.beginGroup("window");
    resize( setting.value( "width", 800 ).toInt() ,
            setting.value( "height", 600 ).toInt() );
    move( setting.value( "left" , 100 ).toInt() ,
          setting.value( "top" , 100 ).toInt() );
    setting.endGroup();
    canva->setComputerSockets( setting.value("computer/socketCount",1).toInt() );
    canva->setHubSockets( setting.value("hub/socketCount",4).toInt() );
    canva->setSwitchSockets( setting.value("switch/socketCount",4).toInt() );
    canva->setRouterSockets(setting.value("router/socketCount",4).toInt() );
    canva->setHubManual(setting.value("hub/manual").toBool());
    canva->setSwitchManual(setting.value("switch/manual").toBool());
    canva->setTtlArp(setting.value("ttl/Arp",1200).toInt());
    canva->setTtlMac(setting.value("ttl/Mac",300).toInt());
    canva->setRip(setting.value("ttl/Rip",30).toInt());
}
//----------------------------------------------------
void MainWindow::groupClicked(QAction *clk)
{
    int m = clk->data().toInt() / 10;
    int t = clk->data().toInt() % 10;
    canva->setMode(m,t);
}

void MainWindow::uncheck()
{
    moveAct->setChecked(true);
}

void MainWindow::test()
{
    testDialog *t = new testDialog(canva);
    t->exec();
    delete t;
}

void MainWindow::addInterface()
{
    interfaceDialog *d = new interfaceDialog;
    d->move(400,400);
    if ( d->exec() ) {
        computer *t = qgraphicsitem_cast<computer*>(canva->oneSelectedDevice());
        t->addInterface( t->nextName() , d->result() );
        selectionChange();
    }
    delete d;
}

void MainWindow::editInterface()
{
    device *d = canva->oneSelectedDevice() ;
    d->dialog();
    selectionChange();
}

void MainWindow::removeInterface()
{
    computer *t = qgraphicsitem_cast<computer*>(canva->oneSelectedDevice());
    devicePort* temp = t->socket( cb_ports->currentText() );
    if ( temp->isConnect() ) {
        QMessageBox::warning(this,trUtf8("Error"),
                             trUtf8("Сначала необходимо отсоединить кабель.") , QMessageBox::Ok , QMessageBox::Ok);
    } else {
        t->removeSocket(temp);
        selectionChange();
    }
}

void MainWindow::tableShow()
{
    device *d = canva->oneSelectedDevice();
    if ( d->type() == computer::Type || d->type() == routerDevice::Type ) {
        smartDevice *u = qgraphicsitem_cast<smartDevice*>(d);
        u->editorShow();
        return;
    }
    if ( d->type() == switchDevice::Type ) {
        switchDevice *u = qgraphicsitem_cast<switchDevice*>(d);
        u->dialogTable();
    }
}

void MainWindow::adapterShow()
{
    smartDevice *d = qgraphicsitem_cast<smartDevice*>(canva->oneSelectedDevice());
    d->adapterShow();
}


//Help=)
void MainWindow::showHelp()
{

}

/**
    Слот включает или выключает симуляцю сцены, меняет картинки и подсказки.
 */
void MainWindow::playBack()
{
    if ( canva->isPlayed() ) {
        canva->stop();
        playAct->setIcon(QIcon(":/im/images/play.png"));
        playAct->setToolTip(trUtf8("Запустить"));
        playAct->setStatusTip(trUtf8("Запустить симуляцию"));
    } else {
        canva->play();
        playAct->setIcon(QIcon(":/im/images/pause.png"));
        playAct->setToolTip(trUtf8("Остановить"));
        playAct->setStatusTip(trUtf8("Остановить симуляцию"));
    }
}
//--------------------------------------------------


