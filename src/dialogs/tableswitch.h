#ifndef TABLESWITCH_H
#define TABLESWITCH_H

#include "ui_tableswitch.h"

class switchTableSetting;


class tableSwitch : public QDialog , private Ui::tableSwitch
{
    Q_OBJECT
    Q_DISABLE_COPY(tableSwitch)
public:
    explicit tableSwitch(switchTableSetting *s);
    void correctSize();     
private:
    switchTableSetting *sw;
public slots:
    void addRecord();
    void deleteRecord();
protected:
    virtual void changeEvent(QEvent *e);
    void resizeEvent(QResizeEvent *e) { Q_UNUSED(e); correctSize(); }
};

#endif // TABLESWITCH_H
