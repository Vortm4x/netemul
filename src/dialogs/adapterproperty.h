#ifndef ADAPTERPROPERTY_H
#define ADAPTERPROPERTY_H

#include "dialogtemplate.h"

class QTabBar;
class QLineEdit;
class QCheckBox;
class QLabel;
class QPushButton;

class devicePort;
class ipEdit;
class adapterSetting;

/**
  Класс диалога настройки адаптеров.
*/
class adapterProperty : public dialogTemplate
{
    Q_OBJECT
    Q_DISABLE_COPY(adapterProperty)
public:
    adapterProperty(adapterSetting *s);
    ~adapterProperty();
private:
    adapterSetting *sd;
    QLabel *lb_statics;
    QTabBar *tab_interfaces;
    QLineEdit *le_name;
    QLineEdit *le_mac;
    ipEdit *le_ip;
    ipEdit *le_mask;
    QPushButton *btn_add;
    QPushButton *btn_del;
private slots:
    QIcon connectIcon(bool b);
    void updateTab(int n);
    void addInterface();
    void deleteInterface();
    void reset();
    void changeTab(int n);
public slots:
    void apply();
};

#endif // ADAPTERPROPERTY_H
