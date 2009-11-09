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
#include <QtGui/QDesktopServices>
#include "mainwindow.h"
#include "scenecontrol.h"
#include "settingdialog.h"
#include "testdialog.h"
#include "mycanvas.h"
#include "appsetting.h"
#include "staticsdialog.h"
#include "statisticsscene.h"
#include "logdialog.h"
#include "aboutwindow.h"

#ifndef QT_NO_OPENGL
#include <QtOpenGL/QtOpenGL>
#endif

#define UPDATEACTION(A,TEXT,TIP) A->setText(TEXT); A->setToolTip(TIP); A->setStatusTip(TIP);
#define FILES_CURRENT_TYPES FILES_TYPES_NET

#define FILES_TYPES_XML "XML files(*.xml);;Networks(*.net)"
#define FILES_TYPES_NET "Networks(*.net);;XML files(*.xml)"

// Конструктор главной формы
MainWindow::MainWindow(QWidget *parent, QStringList param) : QMainWindow(parent)
{
    QCoreApplication::setApplicationVersion("0.9.6");
    createAction(); // Создаем события
    createTools(); //
    createMenu(); // Создаем меню
    createScene(); // Создаем сцену
    setEnabledFileItems( myFile.length() );
    view = new QGraphicsView(canva,this);
    view->setFocus(); // Даем ему фокус
    view->setRenderHint(QPainter::Antialiasing); // Включаем сглаживание
    view->setOptimizationFlags( QGraphicsView::DontClipPainter  | QGraphicsView::DontSavePainterState  );
    view->setViewportUpdateMode( QGraphicsView::BoundingRectViewportUpdate );
    view->installEventFilter(this);
    statusBar()->showMessage(""); // Активизируем статус бар
    timeLabel = new QLabel;
    statusBar()->addPermanentWidget(timeLabel);
    readSetting();
    setCentralWidget(view);
    retranslate();
    setOpenglMode( appSetting::hasOpengl() );
    autosaveTimer = new QTimer(this);
    autosaveTimer->start( appSetting::autosaveInterval() * 60000 );
    connect( autosaveTimer , SIGNAL(timeout()) , SLOT(autosave()) );
    printer = 0;
    printerPainter = 0;
    if ( param.size() > 1 ) {
        if ( QFile::exists( param.at(1) ) == false ) exit(0);
        openFile( param.at(1) );
    }
}

MainWindow::~MainWindow()
{
    if ( printer ) {
        delete printer;
    }
}

//Событие закрытия
void MainWindow::closeEvent(QCloseEvent *event)
{
    // При попытке закрыть главное окно выводим запрос на подтверждение
    if ( !canva->isModified() ) {
        writeSetting();
        event->accept();
        return;
    }
    int res = saveFileDialog();
    if (res != QMessageBox::Cancel ) {
        if ( res == QMessageBox::Save )
            if ( !saveFile() ) {
                event->ignore();
                return;
            }
        writeSetting();
        event->accept();
        return;
    }
    else event->ignore();
}

int MainWindow::saveFileDialog()
{
    return QMessageBox::question(this, tr("File was modified") , tr("File was modified, do you want to save changes?"),
                                 QMessageBox::Save  | QMessageBox::Discard | QMessageBox::Cancel , QMessageBox::Save );
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
    UPDATEACTION( openAct , tr("Open..."),tr("Open existing file") )
    UPDATEACTION( saveAct , tr("Save"),tr("Save network") )
    UPDATEACTION( saveAsAct , tr("Save as..."),tr("Save network as...") )
    UPDATEACTION( closeAct , tr("Close"),tr("Close current file") )
    UPDATEACTION( exitAct , tr("Exit"),tr("Exit") )
    UPDATEACTION( showGridAct , tr("Show grid"),tr("Show grid") )
    UPDATEACTION( arpAct , tr("Arp table"), tr("Arp table") );
    UPDATEACTION( deleteAct , tr("Delete"),tr("Deleting object") )
    UPDATEACTION( progAct , tr("Programs"), tr("Programs installed on device") )
    UPDATEACTION( settingAct ,  tr("Settings...") , tr("Settings") )
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
    UPDATEACTION( logAct , tr("Show log") , tr("Show device log file") )
    UPDATEACTION( aboutDeviceAct , tr("About device") , tr("Information about device") )
    UPDATEACTION( designerPacketAct , tr("Packet designer...") , tr("Create user's packet") )
    UPDATEACTION( printAct , tr("Print...") , tr("Print user's network") )
    UPDATEACTION( printPreviewAct , tr("Preview...") , tr("Preview network berfore printing") );
    UPDATEACTION( virtualNetworkAct , tr("Configure VLAN...") , tr("Configure VLAN") );
    UPDATEACTION( noteAct , tr("Set description...") , tr("Set description of device") );
    fileMenu->setTitle(tr("File"));
    editMenu->setTitle(tr("Edit"));
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

    openAct = createOneAction( QIcon(":/im/images/fileload.png") );
    openAct->setShortcut(QKeySequence::Open);
    connect( openAct , SIGNAL(triggered()) , SLOT(openFile()));

    saveAct = createOneAction( QIcon(":/im/images/filesave.png") );
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
    moveAct->setShortcut(tr("Ctrl+1"));

    cableAct = createOneAction(QIcon(":/im/images/cable.png"),true);
    cableAct->setData( myCanvas::cable*10 + myCanvas::noDev );
    cableAct->setShortcut(tr("Ctrl+2"));

    textAct = createOneAction(QIcon(":/im/images/note.png"), true);
    textAct->setData( myCanvas::text*10 + myCanvas::noDev );
    textAct->setShortcut(tr("Ctrl+3"));

    shareBusAct = createOneAction(QIcon(":/im/images/sharebus.png"),true);
    shareBusAct->setData( myCanvas::insert*10 + myCanvas::busDev);

    computerAct = createOneAction(QIcon(":/im/images/computer.png"),true);
    computerAct->setData( myCanvas::insert*10 + myCanvas::compDev);
    computerAct->setShortcut(tr("Ctrl+4"));

    hubAct = createOneAction(QIcon(":/im/images/hub.png"),true);
    hubAct->setData( myCanvas::insert*10 + myCanvas::hubDev );
    hubAct->setShortcut(tr("Ctrl+5"));

    switchAct = createOneAction(QIcon(":/im/images/switch.png"),true);
    switchAct->setData( myCanvas::insert*10 + myCanvas::switchDev );
    switchAct->setShortcut(tr("Ctrl+6"));

    routerAct = createOneAction(QIcon(":/im/images/router.png") , true);
    routerAct->setData( myCanvas::insert*10 + myCanvas::routerDev );
    routerAct->setShortcut(tr("Ctrl+7"));

    sendAct = createOneAction(QIcon(":/im/images/left_right.png"),true);
    sendAct->setData( myCanvas::send* 10 + myCanvas::noDev);
    sendAct->setShortcut(tr("Ctrl+8"));

    testAct = createOneAction();
    connect( testAct , SIGNAL(triggered()) , SLOT(test()));

    propertyAct = createOneAction(QIcon(":/im/images/properties.png") );
    propertyAct->setShortcut(tr("Ctrl+Alt+P"));

    tableAct = createOneAction(QIcon(":/im/images/table_route.png"));
    tableAct->setShortcut( tr("Ctrl+T"));

    aboutQtAct = createOneAction();
    connect( aboutQtAct , SIGNAL(triggered()) , qApp ,SLOT(aboutQt()) );

    aboutAct = createOneAction(QIcon(":/im/images/NetEmul.png"));
    connect( aboutAct , SIGNAL(triggered()) , SLOT(aboutDialog()));

    helpAct = createOneAction(QIcon(":/im/images/question.png"));
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

    logAct = createOneAction( QIcon(":/im/images/log.png"));
    arpAct->setShortcut(tr("Ctrl+L"));

    aboutDeviceAct = createOneAction(QIcon(":/im/images/question.png"));
    aboutDeviceAct->setShortcut(tr("Ctrl+F1"));
    connect( aboutDeviceAct , SIGNAL(triggered()), SLOT(helpDialog()) );

    designerPacketAct = createOneAction( QIcon(":/im/images/cogwheel.png") );

    printAct = createOneAction( QIcon(":/im/images/print.png") );
    connect( printAct , SIGNAL(triggered()) , SLOT(printDialog()) );

    printPreviewAct = createOneAction( QIcon(":/im/images/print_preview.png") );
    connect( printPreviewAct , SIGNAL(triggered()) , SLOT(printPreviewDialog()) );

    virtualNetworkAct = createOneAction();

    noteAct = createOneAction();
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
    fileMenu->addAction(printAct);
    fileMenu->addAction(printPreviewAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(QString());

    viewMenu = menuBar()->addMenu(QString());
    viewMenu->addAction( deviceBar->toggleViewAction() );
    viewMenu->addAction( controlBar->toggleViewAction() );
    viewMenu->addAction(showGridAct);

    itemMenu = menuBar()->addMenu(QString());
    itemMenu->addAction(propertyAct);
    itemMenu->addAction(tableAct);
    itemMenu->addAction(adapterAct);
    itemMenu->addAction(progAct);
    itemMenu->addAction(arpAct);
    itemMenu->addAction(virtualNetworkAct);
    itemMenu->addAction(logAct);
    itemMenu->addAction(designerPacketAct);
    itemMenu->addAction(aboutDeviceAct);
    itemMenu->addAction(noteAct);
    itemMenu->addAction(deleteAct);
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
    //controlBar->addAction(virtualNetworkAct);
    controlBar->addAction(logAct);
    controlBar->addAction(designerPacketAct);
    controlBar->addSeparator();
    controlBar->setEnabled(false);
    //1ughyh8ihuttji
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
    connect( canva , SIGNAL(selectionChanged()) , SLOT(selectionChange()));
    connect( canva , SIGNAL(fileClosed()) , SLOT(closeFile()) );
    connect( canva , SIGNAL(fileOpened()) , SLOT(newFile()) );
    connect( canva , SIGNAL(tictac()) , SLOT(incTime()) );
    sceneControler = new sceneControl(this,canva);
    connect( sceneControler , SIGNAL(selectOneDevice(bool)) , itemMenu , SLOT(setEnabled(bool)) );
    connect( sceneControler , SIGNAL(selectOneDevice(bool)) , controlBar , SLOT(setEnabled(bool)) );
    connect( sceneControler , SIGNAL(selectOneDevice(bool)) , logAct , SLOT(setVisible(bool)) );
    connect( sceneControler , SIGNAL(selectTableDevice(bool)) , tableAct , SLOT(setVisible(bool)) );
    connect( sceneControler , SIGNAL(selectSmartDevice(bool)) , adapterAct , SLOT(setVisible(bool)) );
    connect( sceneControler , SIGNAL(selectSmartDevice(bool)) , progAct , SLOT(setVisible(bool)) );
    connect( sceneControler , SIGNAL(selectSmartDevice(bool)) , arpAct , SLOT(setVisible(bool)) );
    connect( sceneControler , SIGNAL(selectVirtualNetworkDevice(bool)) , virtualNetworkAct , SLOT(setVisible(bool)) );
    connect( sceneControler , SIGNAL(selectComputer(bool)) , designerPacketAct , SLOT(setVisible(bool)) );
    connect( designerPacketAct , SIGNAL(triggered()) , sceneControler , SLOT(showDesignerDialog()) );
    connect( adapterAct , SIGNAL(triggered()) , sceneControler , SLOT(adapterDialog()) );
    connect( tableAct , SIGNAL(triggered()) , sceneControler , SLOT(tableDialog()) );
    connect( propertyAct , SIGNAL(triggered()) ,sceneControler , SLOT(propertyDialog()) );
    connect( progAct , SIGNAL(triggered()) , sceneControler , SLOT(programmsDialog()) );
    connect( arpAct , SIGNAL(triggered()) , sceneControler , SLOT(arpDialog()) );
    connect( logAct , SIGNAL(triggered()) , SLOT(showLogDialog()) );
    connect( noteAct , SIGNAL(triggered()) , sceneControler , SLOT(showDeviceNoteDialog()) );
    connect( virtualNetworkAct , SIGNAL(triggered()) , sceneControler , SLOT(showVirtualNetworkDialog()) );
    QAction *t = canva->undoAction(this);
    t->setIcon( QIcon(":/im/images/undo.png") );
    t->setShortcut(QKeySequence::Undo);
    editMenu->addAction( t );
    t = canva->redoAction(this);
    t->setShortcut(QKeySequence::Redo);
    t->setIcon( QIcon(":/im/images/redo.png") );
    editMenu->addAction( t );
}
//------------------------------------------------------------------

void MainWindow::showLogDialog()
{
    QString str = sceneControler->note();
    QDockWidget *dock = new QDockWidget( str.remove(QRegExp("<[^>]+>")) );
    dock->setAttribute(Qt::WA_DeleteOnClose);
    logDialog *d = new logDialog( sceneControler->sockets() );
    dock->setWidget(d);
    sceneControler->showLogDialog(d);
    addDockWidget(Qt::BottomDockWidgetArea ,dock);
}

/*!
  Подготавливает окно для работы.
*/
void MainWindow::newFile()
{
    setEnabledFileItems(true);
    showGridAct->setChecked(true);
    canva->newFile();
}
//----------------------------------------------------------
/*!
  Делает окно закрытым для работы.
*/
void MainWindow::closeFile()
{
    if ( !canva->isOpen() ) return;
    if ( canva->isModified() ) {
        int res = saveFileDialog();
        if ( res == QMessageBox::Cancel ) return;
        if ( res == QMessageBox::Save ) saveFile();
    }
    setWindowTitle(myFile = tr(""));
    setEnabledFileItems(false);
    canva->closeFile();
    uncheck();
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
    printAct->setEnabled(cur);
    printPreviewAct->setEnabled(cur);
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
    d->exec();
    canva->setAnimateSpeed( appSetting::animateSpeed() );
    setOpenglMode( appSetting::hasOpengl() );
    autosaveTimer->start( appSetting::autosaveInterval() * 60000 );
}

void MainWindow::setOpenglMode(bool mode)
{
#ifndef QT_NO_OPENGL
    QWidget *widget = 0;
    if ( mode ) widget = new QGLWidget(QGLFormat(QGL::SampleBuffers));
    view->setViewport(widget);
#endif
}

//Слот сохранить
bool MainWindow::saveFile()
{
    if ( myFile.isEmpty() ) {
       saveAsFile();
       return true;
    }
    setWindowTitle( myFile );
    if ( myFile.endsWith("net") ) {
        canva->saveScene(myFile);
    }
    else {
        canva->saveSceneXml(myFile);
    }
    return true;
}

void MainWindow::openFile(QString name)
{
    setWindowTitle(name);
    setEnabledFileItems(true);
    showGridAct->setChecked(true);
    if ( name.endsWith("net") ) {
        canva->openScene(name);
    }
    else {
        canva->openSceneXml(name);
    }
}

void MainWindow::openFile()
{
    QString t = QFileDialog::getOpenFileName(this,tr("Open"),
                                             QDir::currentPath(),tr(FILES_CURRENT_TYPES));
    if ( t.isEmpty() ) return;
    myFile = t;
    openFile( myFile );
}

//Слот сохранить как =)
void MainWindow::saveAsFile()
{
    QString t = QFileDialog::getSaveFileName(this,tr("Save file as ..."),
                                             QApplication::applicationDirPath(),tr(FILES_CURRENT_TYPES));
    if ( t.isEmpty() ) return ;
    if ( !t.endsWith(".net") && !t.endsWith(".xml") ) t.push_back(".xml");
    myFile = t;
    saveFile();
}

// Фильтр для виев айтем =)
bool MainWindow::eventFilter(QObject *obj,QEvent *event)
{
    if ( obj == view ) {
        if ( event->type() == QEvent::Leave )
            canva->hideState();
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
    QString dest;
    if ( sender() == aboutDeviceAct ) dest = sceneControler->deviceName();
    else dest = "index";
    QString t;
    switch ( appSetting::language() ) {
        case 1: t = "ru"; break;
        default: t = "en";
    }
#ifdef Q_WS_MACX
    QDesktopServices::openUrl(QUrl("file:///" +QCoreApplication::applicationDirPath()
                                   + "/../../../doc/"+t+"/"+dest+".html"));
#endif
#ifdef Q_WS_WIN
    QDesktopServices::openUrl(QUrl("file:///" +QCoreApplication::applicationDirPath()
                                   +"/doc/"+t+"/"+dest+".html"));
#endif
#ifdef Q_WS_X11
    QDesktopServices::openUrl(QUrl(DOC_PATH + t + "/" + dest + ".html"));
#endif
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

void MainWindow::aboutDialog()
{
    aboutWindow *w = new aboutWindow;
    w->show();
}

void MainWindow::printDialog()
{
    if ( !printer ) {
        printer = new QPrinter(QPrinter::HighResolution);
    }
    QPrintDialog dialog(printer,this);
    if ( dialog.exec() == QDialog::Accepted ) {
        printer->setDocName(file());
        printerPainter = new QPainter(printer);
        view->render(printerPainter);
        printerPainter->end();
    }
}

void MainWindow::paintInPreviewDialog(QPrinter *printer)
{
    printer->newPage();
    QPainter painter(printer);
    view->render(&painter);
}

void MainWindow::printPreviewDialog()
{
    if ( !printer ) {
        printer = new QPrinter(QPrinter::HighResolution);
    }
    QPrintPreviewDialog dialog(printer,this);
    connect( &dialog , SIGNAL(paintRequested(QPrinter*)) , SLOT(paintInPreviewDialog(QPrinter*)));
    dialog.exec();
}

void MainWindow::autosave()
{
    if ( appSetting::isAutosave()  && canva->isOpen() ) saveFile();
}

void MainWindow::incTime()
{
    static QTime t(0,0,0);
    static int n = 0;
    timeLabel->setText( t.addSecs(++n).toString("hh:mm:ss") );
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

