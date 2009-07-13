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
    QPlainTextEdit *te_text;
    QCheckBox *cb_route;
    QComboBox *cb_count;
public slots:
    void apply();
};

#endif // ROUTERPROPERTY_H
