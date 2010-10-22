#ifndef SPOOFINGPROPERTY_H
#define SPOOFINGPROPERTY_H

#include <QDialog>

class SpoofingProgram;

namespace Ui {
    class spoofingProperty;
}

class spoofingProperty : public QDialog {
    Q_OBJECT
public:
    spoofingProperty(QWidget *parent = 0);
    ~spoofingProperty();

    void setProgramm(SpoofingProgram *p);

protected:
    void changeEvent(QEvent *e);
private slots:
    void apply();
private:

    SpoofingProgram *pr;
    Ui::spoofingProperty *ui;
};

#endif // SPOOFINGPROPERTY_H
