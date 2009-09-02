#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#if QT_VERSION < 0x040500
#include <QIcon>
#endif

class myCanvas;
class settingDialog;
class sceneControl;

class QActionGroup;
class QToolBar;
class QComboBox;
class QMenu;
class QAction;
class QGraphicsView;

/*!
    Реализует главное окно программы.
*/
class MainWindow : public QMainWindow
{
    Q_OBJECT
    Q_DISABLE_COPY(MainWindow)
public:
    enum { noDev = 0 , busDev = 2 ,compDev = 3 , hubDev = 4 , switchDev = 5 , routerDev = 7 };
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString& file() { return myFile; } // Получить имя текущего файла
    void setFile(const QString &text) { myFile = text; } // Задать текущий файл
    void setEnabledFileItems(bool cur);
    void writeSetting();
    void readSetting();
public slots:
    void newFile();  // Слот создания нового файла
    void closeFile();
    void saveFile();
    void saveAsFile();
    void openFile();
    void setting();
    void test();
    void selectionChange();
    void groupClicked(QAction *clk);
    void uncheck();
    void helpDialog();
    void playBack();
    void statistics();
    void showLogDialog();
    void aboutDialog();
protected:
      void closeEvent(QCloseEvent *event); // Событие вызываемое при попытке закрыть окно
      bool eventFilter(QObject *obj, QEvent *event);
      void changeEvent(QEvent *e);
private:
        QString myFile; // Имя текущего файла
        myCanvas *canva;
        sceneControl *sceneControler;
        QGraphicsView *view; // Графическое представление нашей сцены

        void createMenu(); // Функция создания меню
        void createAction(); // -- событий
        void createScene(); // -- сцены
        void createTools(); // -- Панелей инструментов =)
        void retranslate();

        QAction* createOneAction( QIcon icon = QIcon(),bool inGroup = false);

        QMenu *fileMenu; // Выпадающие меню: файл
        QMenu *viewMenu; // -- вид
        QMenu *itemMenu; // -- объект
        QMenu *settingMenu; // -- Сервис
        QMenu *testMenu;
        QMenu *helpMenu;
        QToolBar *deviceBar; //
        QToolBar *controlBar; //
        QActionGroup *groupAct;
        QAction *newAct; // События : создать новый файл
        QAction *openAct; // Открыть файл
        QAction *saveAct; // сохранить
        QAction *saveAsAct; // сохранить как
        QAction *closeAct; // закрыть файл
        QAction *exitAct; // Выйти из программы
        QAction *showGridAct; // Отображать сетку или нет
        QAction *deleteAct; // удалить выделенные объекты
        QAction *tableAct;
        QAction *settingAct;
        QAction *staticsAct; // Don't forreget !
        QAction *testAct;
        QAction *computerAct;
        QAction *moveAct;
        QAction *cableAct;
        QAction *textAct;
        QAction *switchAct;
        QAction *routerAct;
        QAction *hubAct;
        QAction *shareBusAct;
        QAction *sendAct;
        QAction *propertyAct;
        QAction *aboutQtAct;
        QAction *aboutAct;
        QAction *helpAct;
        QAction *adapterAct;
        QAction *playAct;
        QAction *progAct;
        QAction *arpAct;
        QAction *logAct;
};

#endif // MAINWINDOW_H
