#ifndef COMPUTERPROPERTY_H
#define COMPUTERPROPERTY_H

#include "dialogtemplate.h"

class QCheckBox;

class computer;
class ipEdit;

class computerProperty : public dialogTemplate
{
    Q_OBJECT
public:
    computerProperty();
    void setComputer(computer *c);
private:
    computer *comp;
    QPushButton *btn_adapter;
    QPushButton *btn_table;
    QCheckBox *cb_route;
    ipEdit *gateway;
public slots:
    void apply();
    void adapterDialog();
    void tableDialog();
};

#endif // COMPUTERPROPERTY_H
