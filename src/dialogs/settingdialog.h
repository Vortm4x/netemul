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
    QSpinBox *sp_ttlMac;
public slots:
    void apply();
};

#endif // SETTINGDIALOG_H
