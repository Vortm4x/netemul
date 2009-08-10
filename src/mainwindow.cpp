#include <QtGui/QGraphicsView>
#include <QtGui/QApplication>
#include <QtGui/QDockWidget>
#include <QtGui/QMenu>
#include <QtGui/QAction>
#include <QtGui/QActionGroup>
#include <QtGui/QStatusBar>
#include <QtGui/QMessageBox>
#include <QtGui/QMenuBar>
#include <QtCore/QEvent>
#include <QtGui/QCloseEvent>
#include <QtCore/QSettings>
#include <QtGui/QFileDialog>
#include <QtGui/QToolBar>
#include <QtGui/QComboBox>
#include "mainwindow.h"
#include "scenecontrol.h"
#include "settingdialog.h"
#include "testdialog.h"
#include "mycanvas.h"
#include "appsetting.h"
#include "staticsdialog.h"
#include "statisticsscene.h"


#ifndef QT_NO_OPENGL
#include <QtOpenGL/QtOpenGL>
#else
#endif

#define UPDATEACTION(A,TEXT,TIP) A->setText(TEXT); A->setToolTip(TIP); A->setStatusTip(TIP);

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
    view->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
#endif
    view->installEventFilter(this);
    statusBar()->showMessage(""); // Активизируем статус бар
    readSetting();
    setCentralWidget(view);
    retranslate();
}

MainWindow::~MainWindow()
{

}

//Событие закрытия
void MainWindow::closeEvent(QCloseEvent *event)
{
    // При попытке закрыть главное окно выводим запрос на подтверждение
    int res = QMessageBox::question(this,tr("Exit"),tr("You want to go?"),
                                    QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if (res == QMessageBox::Yes) {
        writeSetting();
        event->accept();
    }
    else event->ignore();
}

// Создать один Action
QAction* MainWindow::createOneAction(QIcon icon/*= QIcon()*/,bool inGroup /*=false*/)
{
    QAction *temp = new QAction(this);
    temp->setIcon(icon);
    if (inGroup) {
        groupAct->addAction(temp);
        temp->setCheckable(true);
    }
    return temp;
}

void MainWindow::retranslate()
{
    UPDATEACTION(newAct , tr("New"),tr("Create new network") )
    UPDATEACTION( openAct , tr("Open"),tr("Open existing file") )
    UPDATEACTION( saveAct , tr("Save"),tr("Save network") )
    UPDATEACTION( saveAsAct , tr("Save as..."),tr("Save network as...") )
    UPDATEACTION( closeAct , tr("Close"),tr("Close current file") )
    UPDATEACTION( exitAct , tr("Exit"),tr("Exit") )
    UPDATEACTION( showGridAct , tr("Show grid"),tr("Show grid") )
    UPDATEACTION( arpAct , tr("Arp table"), tr("Arp table") );
    UPDATEACTION( deleteAct , tr("Delete"),tr("Deleting object") )
    UPDATEACTION( progAct , tr("Programms"), tr("Programms installed on device") )
    UPDATEACTION( settingAct ,  tr("Setting") , tr("Setting") )
    UPDATEACTION( playAct , tr("Stop") , tr("Stop simulation") )
    UPDATEACTION( staticsAct , tr("Statistics ") , tr("Show scene statistics ") )
    UPDATEACTION( moveAct , tr("Move"),tr("Move objects") )
    UPDATEACTION( adapterAct , tr("Netcards") , tr("Edit netcards") )
    UPDATEACTION( cableAct , tr("Cable"),tr("Create connection") )
    UPDATEACTION( textAct , tr("Note") , tr("Insert text comment") )
    UPDATEACTION( helpAct , tr("NetEmul Help") , tr("Full help system program") )
    UPDATEACTION( shareBusAct , tr("Unibus"),tr("Add unibus") )
    UPDATEACTION( computerAct , tr("Computer"),tr("Add computer") )
    UPDATEACTION( aboutAct , tr("About NetEmul") ,tr("About NetEmul") )
    UPDATEACTION( hubAct , tr("Hub"),tr("Add hub") )
    UPDATEACTION( switchAct , tr("Switch"),tr("Add switch") )
    UPDATEACTION( routerAct , tr("Router") , tr("Add router") )
    UPDATEACTION( sendAct , tr("Send"),tr("Send data") )
    UPDATEACTION( testAct , tr("Execute scripts") , tr("Run existing scripts")  )
    UPDATEACTION( aboutQtAct , tr("About Qt") , tr("About Qt") )
    UPDATEACTION( propertyAct , tr("Properties") , tr("Show properties")  )
    fileMenu->setTitle(tr("File"));
    viewMenu->setTitle(tr("View"));
    itemMenu->setTitle(tr("Object"));
    settingMenu->setTitle(tr("Service"));
    testMenu->setTitle(tr("Scripts"));
    helpMenu->setTitle(tr("Help"));
    deviceBar->setWindowTitle(tr("Devices"));
    controlBar->setWindowTitle(tr("Controls"));
}

//Создаем элементы меню
void MainWindow::createAction()
{
    groupAct = new QActionGroup(this);
    groupAct->setExclusive(true);
    connect( groupAct , SIGNAL(triggered(QAction*)) , SLOT(groupClicked(QAction*)));

    newAct = createOneAction();
    newAct->setShortcut(QKeySequence::New); // Устанавливаем горячие клавиши
    connect( newAct, SIGNAL(triggered()) , SLOT(newFile()));

    openAct = createOneAction();
    openAct->setShortcut(QKeySequence::Open);
    connect( openAct , SIGNAL(triggered()) , SLOT(openFile()));

    saveAct = createOneAction();
    saveAct->setShortcut(QKeySequence::Save);
    connect( saveAct , SIGNAL(triggered()) , SLOT(saveFile()) );

    saveAsAct = createOneAction();
#if QT_VERSION >= 0x040500
    saveAsAct->setShortcut(QKeySequence::SaveAs);
#endif
    connect( saveAsAct , SIGNAL(triggered()) , SLOT(saveAsFile()) );

    closeAct = createOneAction();
    closeAct->setShortcut(QKeySequence::Close);
    connect( closeAct , SIGNAL(triggered())  , SLOT(closeFile()));

    exitAct = createOneAction();
    exitAct->setShortcut(tr("Ctrl+X"));
    connect( exitAct , SIGNAL(triggered()) , SLOT(close()) );

    showGridAct = createOneAction();
    showGridAct->setCheckable(true);
    showGridAct->setChecked(true);

    deleteAct = createOneAction(QIcon(":/im/images/not.png"));
    deleteAct->setShortcut(tr("Ctrl+D"));

    settingAct = createOneAction();
    settingAct->setShortcut( tr("Ctrl+Alt+S"));
    connect( settingAct , SIGNAL(triggered()) , SLOT(setting()));

    staticsAct = createOneAction();
    connect( staticsAct , SIGNAL(triggered()) , SLOT(statistics()) );

    moveAct = createOneAction(QIcon(":/im/images/arrow.png"),true);
    moveAct->setData( myCanvas::move*10 + myCanvas::noDev );
    moveAct->setChecked(true);

    cableAct = createOneAction(QIcon(":/im/images/cable.png"),true);
    cableAct->setData( myCanvas::cable*10 + myCanvas::noDev );

    textAct = createOneAction(QIcon(":/im/images/note.png"), true);
    textAct->setData( myCanvas::text*10 + myCanvas::noDev );

    shareBusAct = createOneAction(QIcon(":/im/images/sharebus.png"),true);
    shareBusAct->setData( myCanvas::insert*10 + myCanvas::busDev);

    computerAct = createOneAction(QIcon(":/im/images/computer.png"),true);
    computerAct->setData( myCanvas::insert*10 + myCanvas::compDev);

    hubAct = createOneAction(QIcon(":/im/images/hub.png"),true);
    hubAct->setData( myCanvas::insert*10 + myCanvas::hubDev );

    switchAct = createOneAction(QIcon(":/im/images/switch.png"),true);
    switchAct->setData( myCanvas::insert*10 + myCanvas::switchDev );

    routerAct = createOneAction(QIcon(":/im/images/router.png") , true);
    routerAct->setData( myCanvas::insert*10 + myCanvas::routerDev );

    sendAct = createOneAction(QIcon(":/im/images/left_right.png"),true);
    sendAct->setData( myCanvas::send* 10 + myCanvas::noDev);

    testAct = createOneAction();
    connect( testAct , SIGNAL(triggered()) , SLOT(test()));

    propertyAct = createOneAction(QIcon(":/im/images/properties.png") );
    propertyAct->setShortcut(tr("Ctrl+Alt+P"));

    tableAct = createOneAction(QIcon(":/im/images/table_route.png"));
    tableAct->setShortcut( tr("Ctrl+T"));

    aboutQtAct = createOneAction();
    connect( aboutQtAct , SIGNAL(triggered()) , qApp ,SLOT(aboutQt()) );

    aboutAct = createOneAction();

    helpAct = createOneAction();
    helpAct->setShortcut(QKeySequence::HelpContents);
    connect(helpAct,SIGNAL(triggered()) , SLOT(helpDialog()));

    adapterAct = createOneAction(QIcon(":/im/images/network_nic.png"));
    adapterAct->setShortcut(tr("Ctrl+I"));

    playAct = createOneAction( QIcon(":/im/images/pause.png") );
    connect( playAct , SIGNAL(triggered()) ,SLOT(playBack()) );

    progAct = createOneAction( QIcon(":/im/images/install_apps.png"));
    progAct->setShortcut(tr("Ctrl+P"));

    arpAct = createOneAction( QIcon(":/im/images/table_arp.png"));
    arpAct->setShortcut(tr("Ctrl+Shift+A"));
}

//Создаем меню
void MainWindow::createMenu()
{
    // Создаем наши меню и добавляем их на меню бар,
    fileMenu = menuBar()->addMenu(QString());
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addAction(closeAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    viewMenu = menuBar()->addMenu(QString());
    viewMenu->addAction( deviceBar->toggleViewAction() );
    viewMenu->addAction( controlBar->toggleViewAction() );
    viewMenu->addAction(showGridAct);

    itemMenu = menuBar()->addMenu(QString());
    itemMenu->addAction(deleteAct);
    itemMenu->addAction(propertyAct);
    itemMenu->addAction(tableAct);
    itemMenu->addAction(adapterAct);
    itemMenu->addAction(progAct);
    itemMenu->addAction(arpAct);
    itemMenu->setEnabled(false);

    settingMenu = menuBar()->addMenu( QString() );
    settingMenu->addAction(staticsAct);
    settingMenu->addAction(settingAct);

    testMenu = menuBar()->addMenu( QString() );
    testMenu->addAction( testAct );

    helpMenu = menuBar()->addMenu( QString() );
    helpMenu->addAction(helpAct);
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}
/*!
  Создает панели инструментов.
*/
void MainWindow::createTools()
{
    deviceBar = addToolBar(QString());
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
    controlBar = addToolBar(QString());
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
    setWindowTitle(myFile = tr(""));
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
        tableAct->setIcon( sceneControler->tableIcon() );
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
       QString t = QFileDialog::getSaveFileName(this,tr("Save file as ..."),
                                                QDir::currentPath(),
                                                tr("Networks(*.net)"));
       if ( t.isEmpty() ) return;
       myFile = t;
    }
    setWindowTitle( myFile );
    canva->saveScene(myFile);
}

void MainWindow::openFile()
{
    QString t = QFileDialog::getOpenFileName(this,tr("Open"),
                                             QDir::currentPath(),tr("Networks(*.net)"));
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
    QString t = QFileDialog::getSaveFileName(this,tr("Save file as ..."),
                                             QApplication::applicationDirPath(),tr("Networks(*.net)"));
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
        playAct->setToolTip(tr("Play"));
        playAct->setStatusTip(tr("Start simulation"));
    } else {
        canva->play();
        playAct->setIcon(QIcon(":/im/images/pause.png"));
        playAct->setToolTip(tr("Stop"));
        playAct->setStatusTip(tr("Stop simulation"));
    }
}
//--------------------------------------------------

/*!
  * Выводит на экран статистику программы.
  */
void MainWindow::statistics()
{
    statisticsScene s(canva);
    staticsDialog *d = new staticsDialog(&s);
    d->exec();
}

void MainWindow::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslate();
        break;
    default:
        break;
    }
}

