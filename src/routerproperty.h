#ifndef ROUTERPROPERTY_H
#define ROUTERPROPERTY_H

#include "dialogtemplate.h"

class routerDevice;
class QCheckBox;
class QGroupBox;
class QComboBox;
class QPlainTextEdit;

class routerProperty : public dialogTemplate
{
    Q_OBJECT
    Q_DISABLE_COPY(routerProperty)
public:
    routerProperty();
    void setRouter(routerDevice *r);
private:
    routerDevice *rt;
    QPushButton *btn_adapter;
    QPushButton *btn_table;
    QPushButton *btn_arp;
    QPlainTextEdit *te_text;
    QCheckBox *cb_route;
    QComboBox *cb_count;
public slots:
    void apply();
    void tableDialog();
    void adapterDialog();
    void arpDialog();
};

#endif // ROUTERPROPERTY_H
