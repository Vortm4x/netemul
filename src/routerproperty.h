#ifndef ROUTERPROPERTY_H
#define ROUTERPROPERTY_H

#include "dialogtemplate.h"

class routerDevice;
class QRadioButton;
class QGroupBox;

class routerProperty : public dialogTemplate
{
    Q_OBJECT
public:
    routerProperty();
    void setRouter(routerDevice *r);
private:
    void createButtons();
    routerDevice *rt;
    QPushButton *btn_adapter;
    QPushButton *btn_table;
    QPushButton *btn_arp;
    QRadioButton *rtn_route[3];
    QGroupBox *box_group;
public slots:
    void apply();
    void tableDialog();
    void adapterDialog();
    void arpDialog();
};

#endif // ROUTERPROPERTY_H
