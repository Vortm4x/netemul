#ifndef HUBPROPERTY_H
#define HUBPROPERTY_H

#include "dialogtemplate.h"
class QComboBox;
class QCheckBox;
class QLineEdit;
class QLabel;
class QPlainTextEdit;

class ipEdit;
class hubDevice;

class hubProperty : public dialogTemplate
{
Q_OBJECT
public:
    hubProperty();
    void setHub(hubDevice *h);
private:
    hubDevice *hub;
    QComboBox *cb_count;
    QCheckBox *chk_manual;
    QLabel *lb_recFrame;
    QLabel *lb_sendFrame;
    QLabel *lb_recPacket;
    QLabel *lb_sendPacket;
    QLabel *lb_mac;
    QLineEdit *le_mac;
    QPlainTextEdit *te_text;
    ipEdit *le_ip;
    ipEdit *le_mask;
public slots:
    void check(bool p);
    void apply();
};

#endif // HUBPROPERTY_H
