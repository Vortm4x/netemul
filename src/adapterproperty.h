#ifndef ADAPTERPROPERTY_H
#define ADAPTERPROPERTY_H

#include "dialogtemplate.h"

class QTabBar;
class QLineEdit;
class QCheckBox;
class QLabel;

class smartDevice;
class devicePort;
class ipEdit;

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
    void setSmart(smartDevice *r);
    void updateTab(devicePort *d);
private:
    smartDevice *sd;
    QLabel *lb_recFrame;
    QLabel *lb_recPacket;
    QLabel *lb_sendFrame;
    QLabel *lb_sendPacket;
    QTabBar *tab_interfaces;
    QLineEdit *le_name;
    QLineEdit *le_mac;
    ipEdit *le_ip;
    ipEdit *le_mask;
public slots:
    void changeTab();
    void reset();
    void apply();
};

#endif // ADAPTERPROPERTY_H
