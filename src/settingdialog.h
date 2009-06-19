#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include "dialogtemplate.h"
class QListWidget;
class QStackedWidget;
class QWidget;
class QComboBox;
class QCheckBox;
class QSpinBox;

static const int COUNT = 5;

class settingDialog : public dialogTemplate
{
    Q_OBJECT
public:
    settingDialog();    
    bool huManual() const { return myHubManual; }
    bool switchManual() const { return mySwitchManual; }
    int switchSockets() const { return mySwitchSockets; }
    int computerSockets() const { return myComputerSockets; }
    int routerSockets() const { return myRouterSockets; }
    int hubSockets() const { return myHubSockets; }
    int ttlArp() const { return myTtlArp; }
    int ttlMac() const { return myTtlMac; }
    int rip() const { return myRip; }
    void setRouterSockets(unsigned int i);
    void setSwitchManual(bool b);
    void setSwitchSockets(unsigned int i);
    void setComputerSockets(int i);
    void setHubSockets(unsigned int i);
    void setHubManual(bool b);
    void setTtlArp(int i);
    void setTtlMac(int i);
    void setRip(int i);
    void createGeneral();
    void createComputer();
    void createHub();
    void createSwitch();
    void createRouter();
private:
    QComboBox *computerComboBox;
    QComboBox *hubComboBox;
    QComboBox *switchComboBox;
    QComboBox *routerComboBox;
    QCheckBox *hubCheckBox;
    QCheckBox *switchCheckBox;
    QWidget *widgets[COUNT];
    QVBoxLayout *widgetLayouts[COUNT];
    QListWidget *listWidget;
    QStackedWidget *stackedWidget;
    QSpinBox *sp_ttlArp;
    QSpinBox *sp_rip;
    QSpinBox *sp_ttlMac;
    int myHubSockets;
    int mySwitchSockets;
    int myComputerSockets;
    int myRouterSockets;
    int myTtlArp;
    int myTtlMac;
    int myRip;
    bool myHubManual;
    bool mySwitchManual;
signals:
    void sendApply();
public slots:
    void apply();
};

#endif // SETTINGDIALOG_H
