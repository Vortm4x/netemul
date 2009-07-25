#ifndef DIALOGTEMPLATE_H
#define DIALOGTEMPLATE_H

#include <QDialog>
#include <QPushButton>
#include <QBoxLayout>

class dialogTemplate : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(dialogTemplate)
public:
    dialogTemplate();
protected:
    QPushButton *btn_ok;
    QPushButton *btn_apply;
    QPushButton *btn_cancel;
    QHBoxLayout *lay;
public slots:
    virtual void apply() { }
    void applyEnable();
    void applyDisable();
};

#endif // DIALOGTEMPLATE_H
