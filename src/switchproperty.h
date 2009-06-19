#ifndef SWITCHPROPERTY_H
#define SWITCHPROPERTY_H

#include "dialogtemplate.h"

class QComboBox;
class QCheckBox;
class QLineEdit;
class QLabel;

class switchDevice;
class ipEdit;

class switchProperty : public dialogTemplate
{
Q_OBJECT
public:
    switchProperty();
    void setSwitch(switchDevice *d);
private:
    switchDevice *sw;    
    QComboBox *cb_count;    
    QCheckBox *chk_manual;
    QLabel *lb_recFrame;
    QLabel *lb_sendFrame;
    QLabel *lb_recPacket;
    QLabel *lb_sendPacket;
    QLabel *lb_mac;
    QLineEdit *le_mac;
    ipEdit *le_ip;
    ipEdit *le_mask;
public slots:
    void check(bool p);
    void apply();
};

#endif // SWITCHPROPERTY_H
