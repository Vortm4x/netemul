#include "mainwindow.h"
#include "scenecontrol.h"
#include "settingdialog.h"
#include "testdialog.h"
#include "mycanvas.h"
#include "appsetting.h"
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
    QCoreApplication::setApplicationVersion("0.8.1");
    createAction(); // Создаем события
    createTools(); //
    createMenu(); // Создаем меню
    createScene(); // Создаем сцену
    setEnabledFileItems( myFile.length() );
    view = new QGraphicsView(canva,this);
    view->setFocus(); // Даем ему фокус
    view->setRenderHint(QPainter::Antialiasing); // Включаем сглаживание
//    view->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    view->setOptimizationFlags( QGraphicsView::DontClipPainter  | QGraphicsView::DontSavePainterState  );
    view->setViewportUpdateMode( QGraphicsView::BoundingRectViewportUpdate );
#ifndef QT_NO_OPENGL
#if USER != frost
    view->setViewport(new QGLWidget(QGLFormat(QGL::DoubleBuffer)));
#endif
#else
#endif
    view->installEventFilter(this);
    statusBar()->showMessage(""); // Активизируем статус бар
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

    deleteAct = createOneAction(trUtf8("Удалить"),trUtf8("Удалить объект") , QIcon(":/im/images/not.png"));
    deleteAct->setShortcut(tr("Ctrl+D"));

    settingAct = createOneAction( trUtf8("Настройки") , trUtf8("Настройки"));
    settingAct->setShortcut( tr("Ctrl+Alt+S"));
    connect( settingAct , SIGNAL(triggered()) , SLOT(setting()));

    staticsAct = createOneAction( trUtf8("Статистика") , trUtf8("Посмотреть статистику сцены") );

    moveAct = createOneAction( trUtf8("Перемещение"),trUtf8("Перемещение объектов"),
                               QIcon(":/im/images/arrow.png"),true);
    moveAct->setData( myCanvas::move*10 + myCanvas::noDev );
    moveAct->setChecked(true);

    cableAct = createOneAction( trUtf8("Кабель"),trUtf8("Создать соединение"),
                            QIcon(":/im/images/cable.png"),true);
    cableAct->setData( myCanvas::cable*10 + myCanvas::noDev );

    textAct = createOneAction( trUtf8("Текст") , trUtf8("Вставить текстовый комментарий") ,
                               QIcon(":/im/images/note.png"), true);
    textAct->setData( myCanvas::text*10 + myCanvas::noDev );

    shareBusAct = createOneAction( trUtf8("Общая шина"),trUtf8("Добавить общую шину"),
                                    QIcon(":/im/images/sharebus.png"),true);
    shareBusAct->setData( myCanvas::insert*10 + myCanvas::busDev);

    computerAct = createOneAction( trUtf8("Компьютер"),trUtf8("Добавить компьютер"),
                               QIcon(":/im/images/computer.png"),true);
    computerAct->setData( myCanvas::insert*10 + myCanvas::compDev);

    hubAct = createOneAction( trUtf8("Концентратор"),trUtf8("Добавить концентратор"),
                          QIcon(":/im/images/hub.png"),true);
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

    testAct = createOneAction( trUtf8("Выполнить сценарий") , trUtf8("Запустить сценарий") );
    connect( testAct , SIGNAL(triggered()) , SLOT(test()));

    propertyAct = createOneAction( trUtf8("Свойства") , trUtf8("Свойства") ,
                                  QIcon(":/im/images/properties.png") );
    propertyAct->setShortcut(tr("Ctrl+Alt+P"));


    tableAct = createOneAction( trUtf8("Таблица маршрутизации") , trUtf8("Редактирование таблицы маршрутизации"),
                                QIcon(":/im/images/table_route.png"));
    tableAct->setShortcut( tr("Ctrl+T"));

    aboutQtAct = createOneAction( trUtf8("About Qt") , trUtf8("Справочная информация о Qt") );
    connect( aboutQtAct , SIGNAL(triggered()) , qApp ,SLOT(aboutQt()) );

    aboutAct = createOneAction( trUtf8("О программе") ,trUtf8("О программе") );    

    helpAct = createOneAction( trUtf8("Помощь") , trUtf8("Полная справочная система программы") );
    helpAct->setShortcut(QKeySequence::HelpContents);
    connect(helpAct,SIGNAL(triggered()) , SLOT(helpDialog()));

    adapterAct = createOneAction( trUtf8("Интерфейсы") , trUtf8("Редактировать интерфейсы"),
                                  QIcon(":/im/images/network_nic.png"));
    adapterAct->setShortcut(tr("Ctrl+I"));

    playAct = createOneAction( trUtf8("Остановить") , trUtf8("Остановить симуляцию сцены") ,
                               QIcon(":/im/images/pause.png") );
    connect( playAct , SIGNAL(triggered()) ,SLOT(playBack()) );

    progAct = createOneAction(trUtf8("Программы"), trUtf8("Программы установленные на устройстве"),
                              QIcon(":/im/images/install_apps.png"));
    progAct->setShortcut(tr("Ctrl+P"));

    arpAct = createOneAction(trUtf8("Arp-таблица"), trUtf8("Arp-таблица устройства"),
                              QIcon(":/im/images/table_arp.png"));
    arpAct->setShortcut(tr("Ctrl+Shift+A"));
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
    itemMenu->addAction(propertyAct);
    itemMenu->addAction(tableAct);
    itemMenu->addAction(adapterAct);
    itemMenu->addAction(progAct);
    itemMenu->addAction(arpAct);
    itemMenu->setEnabled(false);

    settingMenu = menuBar()->addMenu( trUtf8("Сервис") );
    settingMenu->addAction(staticsAct);
    settingMenu->addAction(settingAct);

    testMenu = menuBar()->addMenu( trUtf8("Сценарии") );
    testMenu->addAction( testAct );

    helpMenu = menuBar()->addMenu( trUtf8("Справка") );
    helpMenu->addAction(helpAct);
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}
/*!
  Создает панели инструментов.
*/
void MainWindow::createTools()
{
    deviceBar = addToolBar(trUtf8("Устройства"));
    deviceBar->setIconSize(QSize(32,32));
    deviceBar->addAction( moveAct);
    deviceBar->addAction(textAct);
    deviceBar->addAction( cableAct);
    //deviceBar->addAction( shareBusAct);
    deviceBar->addAction( computerAct);
    deviceBar->addAction( hubAct);
    deviceBar->addAction( switchAct);
    deviceBar->addAction( routerAct);
    deviceBar->addAction( sendAct);
    deviceBar->addSeparator();
    deviceBar->addAction( playAct);
    //deviceBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    deviceBar->setEnabled(false);
    controlBar = addToolBar(trUtf8("Управление"));
    controlBar->setIconSize(QSize(32,32));
    controlBar->addAction(propertyAct);
    controlBar->addAction(adapterAct);
    controlBar->addAction(progAct);
    controlBar->addAction(tableAct);
    controlBar->addAction(arpAct);
    controlBar->addSeparator();
    controlBar->setEnabled(false);
}
//------------------------------------------------------
/*!
  Создает сцену и проводит сигнально-слотовые соединения с ней.
*/
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
    connect( canva , SIGNAL(fileClosed()) , SLOT(closeFile()) );
    connect( canva , SIGNAL(fileOpened()) , SLOT(newFile()) );
    sceneControler = new sceneControl(this,canva);
    connect( sceneControler , SIGNAL(selectOneDevice(bool)) , itemMenu , SLOT(setEnabled(bool)) );
    connect( sceneControler , SIGNAL(selectOneDevice(bool)) , controlBar , SLOT(setEnabled(bool)) );
    connect( sceneControler , SIGNAL(selectTableDevice(bool)) , tableAct , SLOT(setVisible(bool)) );
    connect( sceneControler , SIGNAL(selectSmartDevice(bool)) , adapterAct , SLOT(setVisible(bool)) );
    connect( sceneControler , SIGNAL(selectSmartDevice(bool)) , progAct , SLOT(setVisible(bool)) );
    connect( sceneControler , SIGNAL(selectSmartDevice(bool)) , arpAct , SLOT(setVisible(bool)) );
    connect( adapterAct , SIGNAL(triggered()) , sceneControler , SLOT(adapterDialog()) );
    connect( tableAct , SIGNAL(triggered()) , sceneControler , SLOT(tableDialog()) );
    connect( propertyAct , SIGNAL(triggered()) ,sceneControler , SLOT(propertyDialog()) );
    connect( progAct , SIGNAL(triggered()) , sceneControler , SLOT(programmsDialog()) );
    connect( arpAct , SIGNAL(triggered()) , sceneControler , SLOT(arpDialog()) );
}
//------------------------------------------------------------------
/*!
  Подготавливает окно для работы.
*/
void MainWindow::newFile()
{
    setEnabledFileItems(true);
    showGridAct->setChecked(true);
}
//----------------------------------------------------------
/*!
  Делает окно закрытым для работы.
*/
void MainWindow::closeFile()
{
    setWindowTitle(myFile = trUtf8(""));
    setEnabledFileItems(false);
}
//-----------------------------------------------------------
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
/*!
    Слот вызываемый при изменении выделения на сцене.
*/
void MainWindow::selectionChange()
{
    if ( sceneControler->isSelect() ) {
        tableAct->setText( sceneControler->tableName() );
        tableAct->setToolTip( sceneControler->tableName() );
    }
}
//------------------------------------------------------------
// Слот окна настроек
void MainWindow::setting()
{
    settingDialog *d = new settingDialog;
    d->applyDisable();
    d->exec();
    delete d ;
    canva->setAnimateSpeed( appSetting::animateSpeed() );
}

//Слот сохранить
void MainWindow::saveFile()
{
    if ( myFile.isEmpty() ) {
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
    setWindowTitle( t );
    setEnabledFileItems(true);
    showGridAct->setChecked(true);
    canva->openScene(t);
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
    appSetting::writeSetting();

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
    appSetting::readSetting();
}
//----------------------------------------------------
/*!
  Устанавливает режим сцены в зависимости от активированного действия.
  @param clk - указатель на действие.
*/
void MainWindow::groupClicked(QAction *clk)
{
    int m = clk->data().toInt() / 10;
    int t = clk->data().toInt() % 10;
    canva->setMode(m,t);
}
//----------------------------------------------------
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

//Help=)
void MainWindow::helpDialog()
{

}

/*!
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

