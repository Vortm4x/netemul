#ifndef SWITCHPROPERTY_H
#define SWITCHPROPERTY_H

#include "dialogtemplate.h"

class QComboBox;
class QLineEdit;
class QLabel;
class QPlainTextEdit;

class boxSetting;
class ipEdit;

class switchProperty : public dialogTemplate
{
    Q_OBJECT
    Q_DISABLE_COPY(switchProperty)
public:
    switchProperty();
    void setSwitch(boxSetting *d);
private:
    boxSetting *sw;
    QComboBox *cb_count;    
    QLabel *lb_statics;
    QLabel *lb_mac;
    QLineEdit *le_mac;
    QPlainTextEdit *te_text;
    ipEdit *le_ip;
    ipEdit *le_mask;
public slots:
    void reset();
    void apply();
};

#endif // SWITCHPROPERTY_H
