#ifndef ADAPTERPROPERTY_H
#define ADAPTERPROPERTY_H

#include "dialogtemplate.h"

class QTabBar;
class QLineEdit;
class QCheckBox;
class QLabel;

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
    adapterProperty();
    ~adapterProperty();
    void setDevice(adapterSetting *s);
private:
    QIcon connectIcon(bool b);
    void updateTab(int n);
    adapterSetting *sd;
    QLabel *lb_statics;
    QTabBar *tab_interfaces;
    QLineEdit *le_name;
    QLineEdit *le_mac;
    ipEdit *le_ip;
    ipEdit *le_mask;
private slots:
    void reset();
    void changeTab(int n);
public slots:
    void apply();
};

#endif // ADAPTERPROPERTY_H
