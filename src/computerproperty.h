#ifndef COMPUTERPROPERTY_H
#define COMPUTERPROPERTY_H

#include "dialogtemplate.h"

class QCheckBox;

class computer;
class ipEdit;
class QPlainTextEdit;

class computerProperty : public dialogTemplate
{
    Q_OBJECT
    Q_DISABLE_COPY(computerProperty)
public:
    computerProperty();
    void setDevice(computer *c);
private:
    computer *comp;
    QCheckBox *cb_route;
    QPlainTextEdit *te_text;
    ipEdit *gateway;
public slots:
    void apply();
};

#endif // COMPUTERPROPERTY_H
