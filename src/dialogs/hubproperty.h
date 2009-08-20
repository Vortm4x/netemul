#ifndef HUBPROPERTY_H
#define HUBPROPERTY_H

#include "dialogtemplate.h"

class QComboBox;
class QCheckBox;
class QLineEdit;
class QLabel;
class QPlainTextEdit;

class ipEdit;
class hubSetting;

class hubProperty : public dialogTemplate
{
Q_OBJECT
public:
    hubProperty();
    ~hubProperty();
    void setHub(hubSetting *s);
private:
    hubSetting *st;
    QComboBox *cb_count;
    QCheckBox *chk_manual;
    QLabel *lb_statics;
    QLabel *lb_mac;
    QLabel *lb_conflict;
    QLineEdit *le_mac;
    QPlainTextEdit *te_text;
    ipEdit *le_ip;
    ipEdit *le_mask;
public slots:
    void check(bool p);
    void apply();
};

#endif // HUBPROPERTY_H
